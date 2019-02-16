create table if not exists groupmsg(
  id integer PRIMARY KEY,
  chatroomnickname TEXT NOT NULL,
  usernickname TEXT NOT NULL,
  msg TEXT NOT NULL,
  time integer,
  chatroomid TEXT NOT NULL,
  userid TEXT NOT NULL
);

create table if not exists users(
  username TEXT PRIMARY KEY,
  password TEXT NOT NULL,
  time integer
);

create table if not exists activegroups(
  groupname TEXT PRIMARY KEY NOT NULL,
  time integer
);
