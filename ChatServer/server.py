#!/usr/bin/python2.7  
# -*- coding: utf-8 -*-  

import os
import sys
import codecs
import base64
import threading
import sqlite3
import time
import thread
from io import BytesIO
import itchat
from itchat.content import *
from PIL import Image
import chardet
from flask import Flask, request, session, g, redirect, url_for, abort, \
     render_template, flash, send_file

app = Flask(__name__)
app.config['SECRET_KEY'] = 'top secret!'
app.config['DATABASE'] = 'WX.db'
app.config['DEBUG']=False
QR_path = u'QR.png'
login_uuid = 0
friends = {}
chatrooms = {}
password = u'123456'


def keep_db():
    with app.app_context():
        db = get_db()
        cu = db.cursor()
        
        timeStamp = int(round(time.time()))-432000
        otherStyleTime = time.strftime("%Y--%m--%d %H:%M:%S", time.localtime(timeStamp))
        sql = "delete from " + "groupmsg where time < " +  str(timeStamp)
        print sql + " " + otherStyleTime
        cu.execute(sql)
        db.commit()
        
        sql = "delete from " + "activegroups where time < " +  str(timeStamp)
        print sql + " " + otherStyleTime
        cu.execute(sql)
        db.commit()
        cu.close()
        db.close()
        timer = threading.Timer(86400, keep_db)
        timer.start()


def get_db():
    """Opens a new database connection if there is none yet for the
    current application context.
    """
    #print hasattr(g, 'sqlite_db')
    #if not (hasattr(g, 'sqlite_db')):
    #    g.sqlite_db = connect_db()
    g.sqlite_db = connect_db()
    return g.sqlite_db

@app.teardown_appcontext
def close_db(error):
    """Closes the database again at the end of the request."""
    if hasattr(g, 'sqlite_db'):
        g.sqlite_db.close()

def connect_db():
    """Connects to the specific database."""
    print app.config['DATABASE']
    rv = sqlite3.connect(app.config['DATABASE'])
    rv.row_factory = sqlite3.Row
    return rv

def init_db():
    with app.app_context():
        db = get_db()
        with app.open_resource('schema.sql', mode='r') as f:
            db.cursor().executescript(f.read())
        db.commit()
        db.close()

@itchat.msg_register(TEXT)
def reply_msg(msg):
    global password
    if(msg[u'ToUserName'] == u'filehelper'):
        if(msg[u'Content'].startswith(u'设置密码')):
            password = msg[u'Content'].replace(u'设置密码',u'')
            itchat.send(password, toUserName=u'filehelper')
            print '设置密码' + str(password)
        if(msg[u'Content'] == u'密码'):
            itchat.send(password, toUserName=u'filehelper')
            print '查看密码' + str(password)


@app.route("/GetVerification")
def GetVerification():
    return password

@itchat.msg_register(TEXT, isGroupChat=True)
def text_reply(msg):
    #print (msg)
    global chatrooms
    with app.app_context():
        chatroom_id = u''            #来自哪个群聊
        chatroom_nick = u''          #来自哪个群聊的昵称
        username  = u''              #来自哪个用户
        nickname = u''               #来自哪个用户的昵称
        
        #如果发送人是自己，则需特殊处理
        if(friends[0][u'UserName'] == msg[u'FromUserName']):
            chatroom_id = msg[u'ToUserName']          
            chatroom_nick = msg[u'User'][u'NickName']   
            username  = friends[0][u'UserName']          
            nickname = friends[0][u'NickName']            
        else:
            chatroom_id = msg[u'FromUserName']          
            chatroom_nick = msg[u'User'][u'NickName']   
            username  = msg[u'ActualUserName']          
            nickname = msg['ActualNickName']            
        
        #print (u"chatroom_id = " + chatroom_id + u"\tchatroom_nick = " + chatroom_nick + u"\tusername = " + username + u"\tnickname = " + nickname)
        
        #如果群聊不在聊天室列表，则重新获取
        if not serachChatroom_id(chatroom_id) :
            print 'fail serachChatroom_id(chatroom_id), update'
            chatrooms = itchat.get_chatrooms(update=True)

        

        #组包数据库
        MsgId = msg[u'MsgId']
        Content = msg[u'Content']
        CreateTime = msg[u'CreateTime']
        roomrenickname = chatroom_nick
        userremarkname = u""
        
        #优先显示备注名
        if not serachFriend_id(username) :
            userremarkname = nickname
        elif serachFriend_id(username)[u'RemarkName'] == u'':
            userremarkname = nickname
        else:
            userremarkname = serachFriend_id(username)[u'RemarkName']

        param=(MsgId,roomrenickname,userremarkname,Content,CreateTime, chatroom_id, username)
        
        
        db = get_db()
        cu = db.cursor()
        sql= "INSERT OR REPLACE into groupmsg values(?,?,?,?,?,?,?)"
        print sql
        cu.execute(sql,param)
        
        sql= "INSERT OR REPLACE into activegroups values(?,?)"
        print sql
        param = (roomrenickname, CreateTime)
        cu.execute(sql,param)
        
        db.commit()
        cu.close()
        db.close()
        
@app.route("/GetActiveGroups")
def GetActiveGroups():
    with app.app_context():
        db = get_db()
        cu = db.cursor()
        sql= "select * from activegroups"
        cu.execute(sql)

        fetch_list = cu.fetchall()
        res = u''
        for row in fetch_list:
            line = u''
            for col in row:
                if(isinstance(col, int)):
                    col = str(col)
                line = line + col + u'\t$$$'
            res = res + line + u'\n'
            
        cu.close()
        db.close()
        return res
        
@app.route("/GetUserGroups")
def GetUserGroups():
    global friends
    global chatrooms
    friends = itchat.get_friends(update=True)
    chatrooms = itchat.get_chatrooms(update=True)
    
    res = u''
    for room in chatrooms:
        if(room[u"UserName"].startswith(u'@@')):
            res = res + room[u"NickName"] +u"\t$$$" + room[u"UserName"] + u"\n"
    return res
        
@app.route("/GetActiveMember")
def GetActiveMember():
    args = request.args
    chatroom_username = args[u'chatroom']
    
    ChatRoomInfo = itchat.update_chatroom(chatroom_username, detailedMember=True)
    total = len(ChatRoomInfo[u'MemberList'])
    
    res = u''
    for i in range(total):
        if ChatRoomInfo[u'MemberList'][i][u'RemarkName'] != u"":
            res = res + ChatRoomInfo[u'MemberList'][i][u'RemarkName'] +u"\t$$$" + ChatRoomInfo[u'MemberList'][i][u'UserName']  +u"\t$$$" + str(1) + u"\n"
        else:
            res = res + ChatRoomInfo[u'MemberList'][i][u'NickName'] +u"\t$$$" + ChatRoomInfo[u'MemberList'][i][u'UserName']+u"\t$$$" +  str(0) + u"\n"

    return res
        
@app.route("/WXGetQR")
def WXGetQR():
    print 'WXGetQR'
    login_uuid = open_QR()
    
    byte_io = BytesIO()
    image = Image.open(QR_path)
    image.save(byte_io, 'png')
    byte_io.seek(0)
    return send_file(byte_io, mimetype='image/png')

def serachFriend_id(serach_str):
    for friend in friends:
        UserName = friend[u"UserName"]
        if(serach_str == UserName):
            return friend
    return {}
    
def serachChatroom_id(serach_str):
    for room in chatrooms:
        UserName = room[u"UserName"]
        if(serach_str == UserName):
            return room
    return {}

def serachFriend_backname(serach_str):
    for friend in friends:
        RemarkName = friend[u"RemarkName"]
        if(serach_str == RemarkName):
            return friend
    return {}
    
def serachChatroom_nickname(serach_str):
    for room in chatrooms:
        NickName = room[u"NickName"]
        if(serach_str == NickName):
            return room
    return {}

def newWXInstance():
    global friends
    global chatrooms
    userInfo = itchat.web_init()
    itchat.show_mobile_login()
    
    friends = itchat.get_friends(update=True)
    chatrooms = itchat.get_chatrooms(update=True)
    itchat.start_receiving()
    itchat.run()
    print('itchat end.')

@app.route("/username")
def username():
    return friends[0][u'NickName']
    
@app.route("/updateFriends")
def updateFriends():
    global friends
    global chatrooms
    friends = itchat.get_friends(update=True)
    chatrooms = itchat.get_chatrooms(update=True)
    return str(friends)

@app.route("/WXConfirmQR")
def WXConfirmQR():
    itchat.logout()
    print "itchat.logout"
    status = itchat.check_login(login_uuid)
    print 'status = ' + str(status) + '\tuuid = ' + str(login_uuid)
    if status == '200':
        thread.start_new_thread(newWXInstance, ())
        print "itchat login succeed"
        return "succeed"
    else:
        print "itchat login fail"
        return "fail"
    

    

def open_QR():
  for get_count in range(10):
    print('Getting uuid')
    uuid = itchat.get_QRuuid()
    while uuid is None: uuid = itchat.get_QRuuid();time.sleep(1)
    print('Getting QR Code')
    try:
        if itchat.get_QR(uuid): break
        elif get_count >= 9:
            print('Failed to get QR Code, please restart the program')
            sys.exit()
    except ZeroDivisionError:
        print 'div 0 error'
    

  print('Please scan the QR Code.  uuid = ' + str(uuid))
  return uuid

@app.route("/WXQueryDb")
def WXQueryDb():
    args = request.args
    print args
    db = get_db()
    cu = db.cursor()
    cu.execute(args[u'sql'])
    db.commit()

    fetch_list = cu.fetchall()
    
    res = u''
    for row in fetch_list:
        line = u''
        for col in row:
            if(isinstance(col, int)):
                col = str(col)
            line = line + col + u'\t$$$'
        res = res + line + u'%%%\n'
    cu.close()
    db.close()
    return res


if __name__ == '__main__':
    init_db()
    keep_db()
    app.run(host='0.0.0.0')
    print "loop_server running!"



