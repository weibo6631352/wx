#include "QGuiWxSetting.h"
#include <QFile>
#include <QApplication>
#include <QMessageBox>
#include <QTextStream>
#include "QwxSetting.h"
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
QGuiWxSetting::QGuiWxSetting(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	CommonHelper::setStyle(this, QApplication::applicationDirPath() + QStringLiteral("/style.qss"));

// 	QPalette palette;
// 	palette.setBrush(QPalette::Window, QBrush(QColor(232, 241, 252)));
// 	setPalette(palette);
}

QGuiWxSetting::~QGuiWxSetting()
{
}


void QGuiWxSetting::LoadSetting()
{
	QwxSetting *wx_setting = QwxSetting::ins();
	const QMap<QString, QSet<QString>>& alias_map = wx_setting->GetAliasMap();
	const std::map<QString, QString, strLenComp>& replace_map = wx_setting->GetReplaceMap();
	const std::set<QString, strLenComp>& delete_set = wx_setting->GetDeleteSet();

	//别名
	for (auto it = alias_map.constBegin(); it != alias_map.constEnd(); ++it)
	{
		QString goods = it.key();
		QString aliass;
		for (auto iterr = it.value().constBegin(); iterr != it.value().constEnd(); ++iterr)
		{
			aliass += *iterr;
			if (iterr != it.value().constEnd() - 1)
			{
				aliass += QStringLiteral(",");
			}
		}
		//设置ui
		int rows = ui.tableWidget_alias->rowCount();
		ui.tableWidget_alias->insertRow(rows);
		ui.tableWidget_alias->setItem(rows, 0, new QTableWidgetItem(goods));
		ui.tableWidget_alias->setItem(rows, 1, new QTableWidgetItem(aliass));
	}

	//替换
	for (auto it = replace_map.cbegin(); it != replace_map.cend(); ++it)
	{
		QString src = it->first;
		QString dst = it->second;

		//设置ui
		int rows = ui.tableWidget_replace->rowCount();
		ui.tableWidget_replace->insertRow(rows);

		ui.tableWidget_replace->setItem(rows, 0, new QTableWidgetItem(src));
		ui.tableWidget_replace->setItem(rows, 1, new QTableWidgetItem(dst));
	}

	//删除
	QString delete_str;
	for (auto it = delete_set.cbegin(); it != delete_set.cend(); ++it)
	{

		delete_str += *it;
		if (std::distance(it, delete_set.cend()) != 1)
		{
			delete_str += QStringLiteral(",");
		}
	}
	//设置ui
	ui.textEdit_nomean_showedit->setText(delete_str);
}

void QGuiWxSetting::on_setalias_add()
{
	ui.tableWidget_alias->insertRow(0);
}

void QGuiWxSetting::on_setalias_del()
{
	int rowIndex = ui.tableWidget_alias->currentRow();
	if (rowIndex != -1)
		ui.tableWidget_alias->removeRow(rowIndex);
}

void QGuiWxSetting::on_setalias_apply()
{
	QwxSetting *wxSetting = QwxSetting::ins();
	QMap<QString, QSet<QString>> alias_map;

	int alias_count = ui.tableWidget_alias->rowCount();
	for (int i = 0; i < alias_count; ++i)
	{
		QString goods = ui.tableWidget_alias->item(i, 0)->text();
		if (QwxSetting::ins()->GetGoodsSet().indexOf(goods) != -1)
		{
			QSet<QString> alias_set;
			QStringList alias_list = ui.tableWidget_alias->item(i, 1)->text().split(QStringLiteral(","));
			for (auto alias : alias_list)
			{
				alias_set.insert(alias);
			}
			alias_map[goods] = alias_set;
		}
		else
		{
			QMessageBox::information(nullptr, QStringLiteral("设置失败"), QStringLiteral("没有这种宝物，缺设置了别名,请修改后再试！") + goods, 0);
			return;
		}
	}
	wxSetting->SetAliasMap(alias_map);

	//写ini
	QString app_path = QApplication::applicationDirPath();
	QFile f;

	//别名设置
	f.setFileName(app_path + QStringLiteral("/alias.ini"));
	if (f.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream txtInput(&f);
		txtInput.setCodec("UTF-8"); //请注意这行
		int alias_count = ui.tableWidget_alias->rowCount();
		for (int i = 0; i < alias_count; ++i)
		{
			txtInput << ui.tableWidget_alias->item(i, 0)->text() << QStringLiteral("=") << ui.tableWidget_alias->item(i, 1)->text() << QStringLiteral("\n");
		}


		f.close();
	}
	else
		QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("alias.ini打开失败!"));
}


void QGuiWxSetting::on_setreplace_add()
{
	ui.tableWidget_replace->insertRow(0);
}

void QGuiWxSetting::on_setreplace_del()
{
	int rowIndex = ui.tableWidget_replace->currentRow();
	if (rowIndex != -1)
		ui.tableWidget_replace->removeRow(rowIndex);
}

void QGuiWxSetting::on_setreplace_apply()
{
	QwxSetting *wxSetting = QwxSetting::ins();
	std::map<QString, QString, strLenComp> replace_map;

	int reapace_count = ui.tableWidget_replace->rowCount();
	for (int i = 0; i < reapace_count; ++i)
	{
		QString src = ui.tableWidget_replace->item(i, 0)->text();
		QString dst = ui.tableWidget_replace->item(i, 1)->text();

		replace_map[src] = dst;
	}
	wxSetting->SetReplaceMap(replace_map);


	//保存ini
	QString app_path = QApplication::applicationDirPath();
	QFile f;

	//别名设置
	f.setFileName(app_path + QStringLiteral("/replace.ini"));
	if (f.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream txtInput(&f);
		txtInput.setCodec("UTF-8"); //请注意这行
		int rowcount = ui.tableWidget_replace->rowCount();
		for (int i = 0; i < rowcount; ++i)
		{
			txtInput << ui.tableWidget_replace->item(i, 0)->text() << QStringLiteral("=") << ui.tableWidget_replace->item(i, 1)->text() << QStringLiteral("\n");;
		}
		f.close();
	}
	else
		QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("alias.ini打开失败!"));
}

void QGuiWxSetting::on_setnomean_add()
{
	QString src_str = ui.textEdit_nomean_showedit->toPlainText();
	QString add_str = ui.lineEdit_nomean_lineedit->text();

	if (!add_str.isEmpty() && !src_str.contains(add_str))
	{
		ui.textEdit_nomean_showedit->setText(src_str + QStringLiteral(",") + add_str);
	}
}

void QGuiWxSetting::on_setnomean_del()
{
	QString src_str = ui.textEdit_nomean_showedit->toPlainText();
	QString del_str = ui.lineEdit_nomean_lineedit->text();
	if (!del_str.isEmpty() && src_str.contains(del_str))
	{
		src_str.replace(QStringLiteral(",") + del_str, QStringLiteral(""));
		src_str.replace(del_str, QStringLiteral(""));
		ui.textEdit_nomean_showedit->setText(src_str);
	}
}

void QGuiWxSetting::on_setnomean_apply()
{
	QwxSetting *wxSetting = QwxSetting::ins();
	std::set<QString, strLenComp> delete_set;

	QString del_str = ui.textEdit_nomean_showedit->toPlainText();
	QStringList delete_list = del_str.split(QStringLiteral(","));
	for (auto deletec : delete_list)
	{
		delete_set.insert(deletec);
	}
	wxSetting->SetDeleteSet(delete_set);



	QString app_path = QApplication::applicationDirPath();
	QFile f;

	f.setFileName(app_path + QStringLiteral("/nomean.ini"));
	if (f.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream txtInput(&f);
		txtInput.setCodec("UTF-8"); //请注意这行
		txtInput << QStringLiteral("无义字符=") << del_str;
		f.close();
	}
	else
		QMessageBox::warning(this, QStringLiteral("警告"), QStringLiteral("nomean.ini打开失败!"));
}
