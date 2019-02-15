#include "wx.h"
#include <QDateTime>
#include <QPair>
#include <QHeaderView>
#include <QMessageBox>
#include <QShortcut>
#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QDir>
#include <QApplication>
#include <QLayoutItem>
#include <QStyle>
#include <QPainter>
#include <QResizeEvent>
#include "QGoodsData.h"
#include "QProgressWidget.h"
#include "QwxSetting.h"
#include "QGuiViewport.h"
#include "QGuiWxSetting.h"
#include "QwxLog.h"
#include "QwxData.h"
#include "QGuiCharts.h"
#include <QFileDialog>

class QDelBtnDelData :public QObjectUserData
{
public:
	QDelBtnDelData(int row) :row_(row){};
	int row_;
};

class CommonHelper
{
public:
	static void setStyle(QWidget *widget, const QString &style) {
		QFile qss(style);
		qss.open(QFile::ReadOnly);
		widget->setStyleSheet(qss.readAll());
		qss.close();
	}
};

wx::wx(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	InitStyle();

	UpdateTitle();
	setWindowFlags(windowFlags()| Qt::WindowMinMaxButtonsHint);
	
	view_port_ = new QGuiViewport(this);
	
	QShortcut* shortcut = new QShortcut(QKeySequence("Ctrl+F"), this);
	connect(shortcut, SIGNAL(activated()), this, SLOT(on_datainput()));
	connect(ui.pushButton_showhide, &QAbstractButton::toggled,/* view_port_, &QWidget::setVisible*/[this]() {
		if (view_port_->isVisible())
			view_port_->setVisible(false);
		else
		{
			view_port_->setVisible(true);
			view_port_->move(this->geometry().topRight());
		}
	});

	QLabel *labelname = new QLabel(QStringLiteral("总计"),this);
	QLabel *labelnum = new QLabel(QStringLiteral("0"), this);
	QFont ft;
	ft.setPointSize(12);
	labelname->setFont(ft);
	labelnum->setFont(ft);
	ui.horizontalLayout_dailijishu->addWidget(labelname);
	ui.horizontalLayout_dailijishu->addWidget(labelnum);
	
	ui.tableWidget_inputlog->verticalHeader()->setVisible(true);

	ui.tableWidget_inputlog->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
	ui.tableWidget_inputlog->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
	ui.tableWidget_inputlog->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
}

wx::~wx()
{
	view_port_->close();
	delete view_port_;
}

void wx::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
	case Qt::Key_Escape:
		break;
	default:
		QDialog::keyPressEvent(event);
	}
}


void wx::closeEvent(QCloseEvent *event)
{
	// QMessageBox::StandardButton button;
	int button;
	button = QMessageBox::question(this, QStringLiteral("退出程序"),
		QString(QStringLiteral("确认退出程序?")),
		QMessageBox::Yes | QMessageBox::No);
	if (button == QMessageBox::No) {
		event->ignore();  //忽略退出信号，程序继续运行
	}
	else if (button == QMessageBox::Yes) {
		event->accept();  //接受退出信号，程序退出
	}
}

void wx::InitStyle()
{
	CommonHelper::setStyle(this, QApplication::applicationDirPath() + QStringLiteral("/wxmain.qss"));
}

void wx::UpdateTitle()
{
	this->setWindowTitle(QwxSetting::ins()->title_ + QwxSetting::ins()->date_ + QwxSetting::ins()->session_+ QStringLiteral("场"));
}


void wx::paintEvent(QPaintEvent *event)
{
	QStyleOption opt;
	opt.init(this);
	QPainter p(this);
	style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}


void wx::on_tabinputtextEdit()
{
	QShortcut* shortcut = (QShortcut*)sender();
	QString hotKey = shortcut->key().toString();
	int tab_index = hotKey.split(QStringLiteral("+"))[1].toInt();

	ui.tabWidget_dataInput->setCurrentIndex(tab_index - 1);
}

void wx::addUser(QString user, QString session, QString date)
{
	if (user.isEmpty() || session.isEmpty() || -1 != ui.tabWidget_dataInput->GetIndexFromTabText(user))
	{
		QMessageBox::information(this, QStringLiteral(""), QStringLiteral("用户不能为空"), 0);
		return;
	}
	QLabel *labelname = new QLabel(user, this);
	QLabel *labelnum = new QLabel(QStringLiteral("0"), this);
	QFont ft;
	ft.setPointSize(12);
	labelname->setFont(ft);
	labelnum->setFont(ft);
	ui.horizontalLayout_dailijishu->addWidget(labelname);
	ui.horizontalLayout_dailijishu->addWidget(labelnum);
	QString str_num = QString::number(ui.tabWidget_dataInput->count() + 1);
	QShortcut* shortcut = new QShortcut(QKeySequence(QStringLiteral("Ctrl+") + str_num), this);
	connect(shortcut, SIGNAL(activated()), this, SLOT(on_tabinputtextEdit()));

	ui.tabWidget_dataInput->setEnabled(true);
	ui.pushButton_datainput->setEnabled(true);
	QwxData::ins()->AddUserData(user, date, session);
	ui.tabWidget_dataInput->AddTab(user);

	ui.lineEdit_setuser->setText(QStringLiteral(""));

	if (view_port_ != nullptr)
	{
		QTabWidget *tabWidget = view_port_->ui.tabWidget;
		tabWidget->insertTab(tabWidget->count(), new QGuiCharts(user), user);
	}
}

void wx::on_addUser()
{
	QString user = ui.lineEdit_setuser->text();
	QString session = QwxSetting::ins()->session_;
	QString date = QwxSetting::ins()->date_;
	addUser(user, session, date);
}

void wx::UpdateDailiJishu()
{
	QMap<QString, QGoodsData*> &user_data = QwxData::ins()->user_data_;
	((QLabel*)ui.horizontalLayout_dailijishu->itemAt(1)->widget())->setText(QString::number(QwxData::ins()->GetTotolData()->totols_));
	
	for (int i = 0; i < ui.tabWidget_dataInput->count(); ++i)
	{
		QString &username = ui.tabWidget_dataInput->GetTabTextFromIndex(i);
		((QLabel*)ui.horizontalLayout_dailijishu->itemAt((i+1) * 2)->widget())->setText(username);
		((QLabel*)ui.horizontalLayout_dailijishu->itemAt((i + 1) * 2 + 1)->widget())->setText(QString::number(user_data[username]->totols_));

	}
}

void wx::on_datainput()
{
	if (!QwxData::ins()->GetUserCount())
	{
		QMessageBox::information(this, QStringLiteral(""), QStringLiteral("请先添加用户"), 0);
		return;
	}
	QwxLog *wxlog = QwxLog::ins();
	QwxData *wxdata = QwxData::ins();
 	QString input = ui.tabWidget_dataInput->GetCurTabWidget()->toPlainText();
	QString user = ui.tabWidget_dataInput->GetCurTabTitle();


	QString  err_label;
	if (QwxData::ins()->GetUserData(user)->InputData(input, err_label))
	{
		UpdateDailiJishu();
		// 更新输入记录
		UpdateOutPutEdit();
		// 删除输入区域
		CleanInputTextEdit();
		// 更新日志
		UpdateLogTableWidget();
		// 刷新chart
		view_port_->on_updateCharts();
		// 设置焦点
		ui.tabWidget_dataInput->SetFocus();
	}
	else
	{
		ui.tabWidget_dataInput->GetCurTabWidget()->setTextColor(QColor(0, 0, 0));
		QTextDocument *document = ui.tabWidget_dataInput->GetCurTabWidget()->document();
		QTextCursor highlight_cursor(document);
		//QTextCursor cursor(document);

		//开始
		//cursor.beginEditBlock();

		QTextCharFormat color_format(highlight_cursor.charFormat());
		
		if (!highlight_cursor.isNull() && !highlight_cursor.atEnd()) {
			//查找指定的文本，匹配整个单词
			color_format.setForeground(Qt::red);
			highlight_cursor = document->find(err_label, highlight_cursor);
			if (!highlight_cursor.isNull()) {
				highlight_cursor.mergeCharFormat(color_format);
			}
		}
		//cursor.endEditBlock();
		//结束
	}
}

void wx::CleanInputTextEdit()
{
	QwxLog *wxlog = QwxLog::ins();
	if (wxlog->GetLastInputSucceed(ui.tabWidget_dataInput->GetCurTabTitle()))
	{
		ui.tabWidget_dataInput->GetCurTabWidget()->setText(QStringLiteral(""));
	}
}

void wx::on_showset()
{
	QGuiWxSetting set;
	set.LoadSetting();
	set.exec();
}

void wx::on_clearData()
{
	if (QMessageBox::Yes == QMessageBox::question(this, QStringLiteral("确认清空？"), QStringLiteral("清空后会无法恢复，请再次确认！"), QMessageBox::Yes, QMessageBox::No))
	{
		ui.textEdit_showoutput->clear();
		ui.tableWidget_inputlog->clear();
		QwxLog::ins()->user_logs_.clear();
		QwxData::ins()->ClearUsersData();
		UpdateDailiJishu();

	}
}

void wx::UpdateOutPutEdit()
{
	QwxLog *wxlog = QwxLog::ins();
	QString output_str = wxlog->GetOutPutEditString(ui.tabWidget_dataInput->GetCurTabTitle());
	if (!output_str.isEmpty())
	{
		//ui.textEdit_showoutput->setText(ui.textEdit_showoutput->toPlainText() + output_str);
		ui.textEdit_showoutput->setHtml(ui.textEdit_showoutput->toHtml() + output_str);
	}

	QTextCursor cursor = ui.textEdit_showoutput->textCursor();
	cursor.movePosition(QTextCursor::End);
	ui.textEdit_showoutput->setTextCursor(cursor);
}

void wx::RevocationUpdateOutPutEdit(QString user)
{
	QwxLog *wxlog = QwxLog::ins();
	QString output_str = wxlog->GetOutPutEditString(user);
	if (!output_str.isEmpty())
	{
		//ui.textEdit_showoutput->setText(ui.textEdit_showoutput->toPlainText() + QStringLiteral("撤回：") + output_str);
		ui.textEdit_showoutput->setHtml(ui.textEdit_showoutput->document()->toHtml() + QStringLiteral("撤回：") + output_str);
	}

	QTextCursor cursor = ui.textEdit_showoutput->textCursor();
	cursor.movePosition(QTextCursor::End);
	ui.textEdit_showoutput->setTextCursor(cursor);
}

void wx::UpdateLogTableWidget()
{
	QwxLog *wxlog = QwxLog::ins();
	QVector<QString> log_v = wxlog->GetLastLog(ui.tabWidget_dataInput->GetCurTabTitle());
	log_v[2].replace('\n', ' ');
	int currow = ui.tableWidget_inputlog->rowCount();
	ui.tableWidget_inputlog->insertRow(currow);
	ui.tableWidget_inputlog->insertRow(currow+1);
	ui.tableWidget_inputlog->insertRow(currow+2);
	
 	ui.tableWidget_inputlog->setSpan(currow, 0, 1, 2);
 	ui.tableWidget_inputlog->setSpan(currow+1, 0, 1, 4);
 	ui.tableWidget_inputlog->setSpan(currow+2, 0, 1, 4);


	QColor color;
	int row_alternate = (currow)/3;
	if (row_alternate == 1)
	{
		color = QColor(222, 222, 222);
	}
	else
	{
		color = row_alternate % 2 ? QColor(222, 222, 222) : QColor(255, 255, 255);
	}

	//user
	QTableWidgetItem *useritem = new QTableWidgetItem(log_v[1]);
	useritem->setBackgroundColor(color);
	ui.tableWidget_inputlog->setItem(currow, 0, useritem);
	ui.tableWidget_inputlog->item(currow, 0)->setBackground(color);
	
	//time
	QTableWidgetItem *timeitem = new QTableWidgetItem(log_v[0]);
	timeitem->setBackgroundColor(color);
	ui.tableWidget_inputlog->setItem(currow, 2, timeitem);

	//src
	auto src = new QTableWidgetItem(log_v[2]);
	src->setToolTip(log_v[2]);
	src->setBackgroundColor(color);
	ui.tableWidget_inputlog->setItem(currow+1, 0, src);

	//dst
	auto dst = new QTableWidgetItem();
	if (log_v.size() == 4)
	{
		dst->setText(log_v[3]);
		dst->setToolTip(log_v[3]);
	}
	dst->setBackgroundColor(color);
	ui.tableWidget_inputlog->setItem(currow + 2, 0, dst);

	QPushButton *delbtn = new QPushButton(QStringLiteral("撤回"), ui.tableWidget_inputlog);
	delbtn->setUserData(0, new QDelBtnDelData(currow));
	connect(delbtn, SIGNAL(clicked()), this, SLOT(RevocationLogTableWidgetItem()));
	ui.tableWidget_inputlog->setCellWidget(currow, 3, delbtn);
	ui.tableWidget_inputlog->scrollToBottom();
}

void wx::RevocationLogTableWidgetItem()
{
 	QPushButton *delbtn = (QPushButton *)sender();
 	delbtn->setText(QStringLiteral(""));
	delbtn->setStyleSheet(QStringLiteral("background:rgba(255,0,0,100);"));
	delbtn->disconnect(delbtn, SIGNAL(clicked()), this, SLOT(RevocationLogTableWidgetItem()));
	QColor disable_col(255, 0, 0, 100);

 	int row = ((QDelBtnDelData*)delbtn->userData(0))->row_;

	QwxData* wxdata = QwxData::ins();

	ui.tableWidget_inputlog->item(row, 0)->setBackgroundColor(disable_col);
	ui.tableWidget_inputlog->item(row, 2)->setBackgroundColor(disable_col);
	for (int i = row + 1; i < row + 3; ++i)
	{
		ui.tableWidget_inputlog->item(i, 0)->setBackgroundColor(disable_col);
	}



	QwxLog *wxlog = QwxLog::ins();
	QString src_str = ui.tableWidget_inputlog->item(row + 1, 0)->text();
	QString input = ui.tableWidget_inputlog->item(row + 2, 0)->text();
	QString user = ui.tableWidget_inputlog->item(row, 0)->text();


	QwxData::ins()->GetUserData(user)->RevocationData(src_str, input);
 	RevocationUpdateOutPutEdit(user);
	UpdateDailiJishu();


	//撤销不滚屏
	//ui.tableWidget_inputlog->scrollToBottom();
	view_port_->on_updateCharts();
 	//刷新日志
 
// 	CleanInputTextEdit();
// 	UpdateLogTableWidget();

	
}

void wx::on_loadChangDi()
{
	QString app_path = QApplication::applicationDirPath();
	QString dir_path = app_path + QStringLiteral("/场地代理系数");
	if (!QDir(dir_path).exists())
		dir_path = app_path;
	QString fileName = QFileDialog::getOpenFileName(this, QStringLiteral("Open File"), dir_path, QStringLiteral("ini (*.ini)"));
	if (fileName.isEmpty())
	{
		return;
	}

	QString localname = QFileInfo(fileName).baseName();

	QFile f;
	f.setFileName(fileName);
	if (f.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QTextStream txtInput(&f);
		txtInput.setCodec("UTF-8"); //请注意这行
		QString lineStr;
		QwxSetting *setting = QwxSetting::ins();
		setting->title_ = localname;

		QMap<QString, double> agencyProfit;
		while (!txtInput.atEnd())
		{
			lineStr = txtInput.readLine();

			QSet<QString> split_set;
			QStringList str_list = lineStr.split(QStringLiteral("="));
			if (str_list.size() == 2)
			{
				QString username = str_list[0];
				agencyProfit[username] = str_list[1].toDouble();

				if (username != QStringLiteral("默认"))
					addUser(username, setting->session_, setting->date_);
			}
		}
		setting->SetAgencyProfitMap(agencyProfit);
		f.close();
	}
	else
	{
		QMessageBox::warning(nullptr, QStringLiteral("错误"), fileName + QStringLiteral("打开失败!"));
		return;
	}

	ui.label_3->setEnabled(true);
	ui.lineEdit_setuser->setEnabled(true);
	ui.pushButton_setuserapply->setEnabled(true);
	UpdateTitle();
}
