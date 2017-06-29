#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->ui->setupUi(this);

    this->ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);                   //设置不显示水平拖动条
    this->ui->tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);                     //设置不显示垂直拖动条
    this->ui->tableWidget->setColumnCount(7);                                                      //设置列数
    this->ui->tableWidget->setRowCount(5);                                                         //设置行数
    for (int i = 0; i < 7; ++i) {
        this->ui->tableWidget->setColumnWidth(i, (this->ui->tableWidget->width() - 30) / 7);       //设置每列宽度
    }
    for (int i = 0; i < 5; ++i) {
        this->ui->tableWidget->setRowHeight(i, (this->ui->tableWidget->height() - 26) / 5);        //设置每行高度
    }

    //Todo 设置水平垂直标题栏
    QStringList horizontaHeader, vertivalHeader;
    horizontaHeader << "星期一" << "星期二" << "星期三" << "星期四" << "星期五" << "星期六" << "星期日";
    vertivalHeader << "1 | 2" << "3 | 4" << "5 | 6" << "7 | 8" << "晚上";
    this->ui->tableWidget->setHorizontalHeaderLabels(horizontaHeader);
    this->ui->tableWidget->setVerticalHeaderLabels(vertivalHeader);

    this->ui->action_2->setDisabled(true);                                                         //设置导出按钮无效
    this->ui->textEdit_1->setText("2");                                                            //设置每天工作人数为2
    this->ui->textEdit_2->setText("2");                                                            //设置每周排班数为2
    this->openFileAction = new QAction(tr("&导入一个文件"), this);                                  //新建一个Action
    this->openFileAction->setShortcut(QKeySequence::Open);
    this->openFileAction->setStatusTip(tr("导入一个 .xls 文件."));                                  //设置状态栏显示文字
    this->openFileAction->setIcon(QIcon(":/icon/resources/images/open-file"));                     //设置工具栏图标
    this->connect(openFileAction, SIGNAL(triggered()), this, SLOT(OpenFile()));                    //工具栏连接Action
    this->connect(this->ui->action_1, SIGNAL(triggered()), this, SLOT(OpenFile()));                //菜单栏连接Action
    this->connect(this->ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(MemberChanged(int)));
    QToolBar *toolBar = this->addToolBar(tr("文件"));                                              //显示工具栏
    toolBar->addAction(openFileAction);
}

MainWindow::~MainWindow()
{
    delete member;
    delete openFileAction;
    delete ui;
}

/*
 * Edit：leafspace
 * Contact:
 *  Phonenumber:18852923073
 *  E-mail:18852923073@163.com
 * LastEdit:
 *  2017.4.29
 * Function:
 *  清理tableWeight里的内容
*/
void MainWindow::clearTable()
{
    for(int i = 0; i < 7; ++i) {
        for(int j = 0; j < 5; ++j) {
            this->ui->tableWidget->setItem(j, i, new QTableWidgetItem(""));
            this->ui->tableWidget->item(j, i)->setTextAlignment(Qt::AlignCenter);                  //设置单元格居中
        }
    }
}

int MainWindow::isDigitStr(QString src)
{
    QByteArray ba = src.toLatin1();
    const char *s = ba.data();

    while(*s && *s>='0' && *s<='9') s++;

    if(*s) {
        return -1;
    } else {
        return 0;
    }
}

/*
 * Edit：leafspace
 * Contact:
 *  Phonenumber:18852923073
 *  E-mail:18852923073@163.com
 * LastEdit:
 *  2017.4.30
 * Function:
 *  提供给排班功能使用，用于查找表格中内容是否有此字串
*/
bool MainWindow::isSubStr(QString str, QString son)
{
    if(str.indexOf(son) != -1) {
        return true;
    } else {
        return false;
    }
}

/*
 * Edit：leafspace
 * Contact:
 *  Phonenumber:18852923073
 *  E-mail:18852923073@163.com
 * LastEdit:
 *  2017.4.30
 * Function:
 *  提供给排班功能使用，用于删除某字符串中的子串
*/
QString MainWindow::wipeStr(QString str, QString son)
{
    QString result_str = str;
    while(this->isSubStr(result_str, son)) {
        int index = result_str.indexOf(son);
        result_str = result_str.mid(0, index) + result_str.mid(index + son.length() + 1);          //提取字串前面和字串后面位置上的串
    }
    return result_str;
}

/*
 * Edit：leafspace
 * Contact:
 *  Phonenumber:18852923073
 *  E-mail:18852923073@163.com
 * LastEdit:
 *  2017.4.30
 * Function:
 *  提供给排班功能使用，用于获取某个字串中包含人员的个数
 *  (注：人数的个数判定由计算QString中包含的空格的数量而决定)
*/
int MainWindow::getStrMemberNum(QString str)
{
    QByteArray qByteArray = str.toLatin1();
    const char *str_char = qByteArray.data();

    int memberNum = 0;
    while(*str_char) {
        if(*str_char == ' ') {
            memberNum++;
        }
        str_char++;
    }
    return memberNum;
}

/*
 * Edit：leafspace
 * Contact:
 *  Phonenumber:18852923073
 *  E-mail:18852923073@163.com
 * LastEdit:
 *  2017.4.29
 * Function:
 *  打开Excel文件并读取数据内容
*/
void MainWindow::OpenFile()
{
    QString path = QFileDialog::getOpenFileName(this, tr("打开一个Excel文件"),
                                                ".", tr("Excel Files(*.xls *.xlsx)"));             //弹出打开文件窗口
    if(path.length() == 0) {                                                                       //没有选择任何文件而退出
        QMessageBox::critical(0, "错误信息", "你没有选择任何文件");
        return ;
    }
    this->filePath = path;

    excel = new QAxObject("Excel.Application");                                                    //从系统活动中寻找Excel程序
    if (excel->isNull()) {
        QMessageBox::critical(0, "错误信息", "没有找到EXCEL应用程序");
        return;
    }
    excel->dynamicCall("SetVisible(bool)", false);                                                 //设置Excel程序不显示界面
    workbooks = excel->querySubObject("WorkBooks");
    workbook = workbooks->querySubObject("Open(QString, QVariant, QVariant)",
                                         this->filePath, 3, true);                                 //打开Excel中所有的工作簿

    if (!workbook) {
        QMessageBox::critical(0, "错误信息", "excel 文件不存在");
        return;
    }

    QAxObject* allSheets = workbook->querySubObject("Worksheets");                                 //查询工作簿中的所有工作表
    int sheetsnum = allSheets->property("Count").toInt();                                          //查询工作表的个数
    delete allSheets;

    if(sheetsnum <= 1) {                                                                           //如果工作表中只有示例
        delete workbook;
        delete workbooks;
        delete excel;
        QMessageBox::critical(0, "错误信息", "Excel文件中没有成员信息");
        return ;
    }

    this->ui->comboBox->clear();
    QAxObject *cell = NULL;
    this->member = new Member[sheetsnum - 1];                                                      //为人员分配控件
    this->memberNumber = sheetsnum - 1;                                                            //保存工作人员个数
    for(int i = 2; i <= sheetsnum; ++i) {
        QAxObject *worksheet = workbook->querySubObject("WorkSheets(int)", i);                     //获取工作簿中第i个工作表
        if(worksheet == NULL) {
            break;
        }

        //Todo 获取此工作表中的参数范围
        QAxObject *usedrange = worksheet->querySubObject("UsedRange");                             //获取该sheet的使用范围对象
        QAxObject *rows = usedrange->querySubObject("Rows");
        QAxObject *columns = usedrange->querySubObject("Columns");

        int intRowStart = usedrange->property("Row").toInt();
        int intColStart = usedrange->property("Column").toInt();
        int intCols = columns->property("Count").toInt();
        int intRows = rows->property("Count").toInt();

        cell = worksheet->querySubObject("Cells(int,int)", 1, 1);                                  //查询[1,1]中的内容
        QString memberName = cell->property("Value").toString();
        memberName = memberName.mid(1, memberName.indexOf("】") - 1);                              //将内容转换为人名
        this->member[i - 2].setUsername(memberName);
        this->ui->comboBox->addItem(memberName);                                                   //再ComboBox中添加此人

        //Todo 读取此人一周的生活周期内容
        for(int j = 3; j < intColStart + intCols; j+=2){
            OneDay oneDay;
            for(int k = 4; k < intRowStart + intRows; k++){
                cell = worksheet->querySubObject("Cells(int,int)", k, j);                          //获取单元格数据内容
                QString value = cell->property("Value").toString();
                if(value.length() != 0) {
                    int row = (k - 4) / 2;
                    switch(row)
                    {
                    case 0: oneDay.forenone_up = value;    break;
                    case 1: oneDay.forenone_down = value;  break;
                    case 2: oneDay.afternone_up = value;   break;
                    case 3: oneDay.afternone_down = value; break;
                    case 4: oneDay.night = value;          break;
                    }
                }
            }
            this->member[i - 2].setDay((j - 3) / 2, oneDay);
        }
    }

    //Todo 关闭系统服务和清理内存
    workbook->dynamicCall("Close (Boolean)", false);
    excel->dynamicCall("Quit (void)");
    delete workbook;
    delete workbooks;
    delete excel;

    this->MemberChanged(0);                                                                        //人为选中ComboBox第一个选项的内容
    QMessageBox::information(NULL, tr("提示"), tr("导入成功！"));
}

/*
 * Edit：leafspace
 * Contact:
 *  Phonenumber:18852923073
 *  E-mail:18852923073@163.com
 * LastEdit:
 *  2017.4.29
 * Function:
 *  当ComboBox中选项出现变化时的监听事件，改变tableWidget中的内容
*/
void MainWindow::MemberChanged(int index)
{
    this->clearTable();
    if(index == this->memberNumber) {
        for(int i = 1; i < 4; ++i) {
            for(int j = 0; j < 7; ++j) {
                this->ui->tableWidget->setItem(i, j, new QTableWidgetItem(this->classAnswer[i][j]));
                this->ui->tableWidget->item(i, j)->setTextAlignment(Qt::AlignCenter);

                if(this->getStrMemberNum(this->classAnswer[i][j]) !=
                        this->ui->textEdit_1->toPlainText().toInt()) {
                    this->ui->tableWidget->item(i, j)->setTextColor(QColor(255, 0, 0));
                    this->ui->tableWidget->item(i, j)->setBackgroundColor(QColor(255, 210, 210));
                }
            }
        }
        return ;
    }

    for(int i = 0; i < 7; ++i) {
        for(int j = 0; j < 5; ++j) {
            this->ui->tableWidget->setItem(j, i, new QTableWidgetItem(this->member[index].getDay(i).getStr(j)));
            this->ui->tableWidget->item(j, i)->setTextAlignment(Qt::AlignCenter);
        }
    }
}

/*
 * Edit：leafspace
 * Contact:
 *  Phonenumber:18852923073
 *  E-mail:18852923073@163.com
 * LastEdit:
 *  2017.4.30
 * Function:
 *  排班函数
*/
void MainWindow::on_pushButton_clicked()
{
    if(this->member == NULL) {
        QMessageBox::critical(0, "错误信息", "您还没导入成员信息");
        return ;
    }

    int daynum = this->ui->textEdit_1->toPlainText().toInt();
    int weekday = this->ui->textEdit_2->toPlainText().toInt();

    for(int i = 0; i < 5; ++i) {
        for(int j = 0; j < 7; ++j) {
            this->classAnswer[i][j] = "";
            this->classAnswerFlag[i][j] = 0;
        }
    }

    //Todo 原则1：上午有空的尽管来
    for(int i = 0; i < 2; ++i) {
        for(int j = 0; j < 5; ++j) {
            for(int k = 0; k < this->memberNumber; ++k) {
                if(this->member[k].getDay(j).getStr(i).length() == 0) {
                    this->classAnswer[i][j] += this->member[k].getUsername() + " ";
                }
            }
        }
    }

    //Todo 原则2:周六周日要全天有空
    for(int i = 5; i < 7; ++i) {
        for(int j = 0, k; j < this->memberNumber; ++j) {
            for(k = 0; k < 5; ++k) {
                if(this->member[j].getDay(i).getStr(k).length() != 0) {
                    break;
                }
            }

            if(k != 5) {
                continue;
            } else {
                for(k = 0; k < 5; ++k) {
                    this->classAnswer[k][i] += this->member[j].getUsername() + " ";
                }
            }
        }
    }

    //Todo 原则3：周一至周五下午必须都有空
    for(int i = 0; i < 5; ++i) {
        for(int j = 0; j < this->memberNumber; ++j) {
            if(this->member[j].getDay(i).getStr(2).length() == 0 &&
                    this->member[j].getDay(i).getStr(3).length() == 0) {
                this->classAnswer[2][i] += this->member[j].getUsername() + " ";
                this->classAnswer[3][i] += this->member[j].getUsername() + " ";
            }
        }
    }

    //Todo 控制每周班数
    for(int i = 0; i < this->memberNumber; ++i) {
        int workday = 0;

        //Todo 计算第i员工每周工作的天数
        for(int j = 0, k; j < 7; ++j) {
            for(k = 0; k < 5; ++k) {
                if(this->isSubStr(this->classAnswer[k][j], this->member[i].getUsername())) {
                    workday++;
                    break;
                }
            }
            if(k != 5) {
                continue;
            }
        }

        if(workday > weekday) {                                                                    //此员工每周工作超出规定班数
            for(int j = 0, k; j < 7; ++j) {
                for(k = 0; k < 5; ++k) {
                    if(this->isSubStr(this->classAnswer[k][j], this->member[i].getUsername()) &&   //当前日期时间内分配了任务 且
                            this->getStrMemberNum(this->classAnswer[k][j]) > daynum) {             //当前日期时间内的人数大于每日所规定的人数
                        this->classAnswer[k][j] = this->wipeStr(this->classAnswer[k][j], this->member[i].getUsername());
                        workday--;
                    }
                }
                if(workday <= weekday) {
                    break;
                }
            }
        }
    }

    //Todo 控制每天上班人数 （注：上方已存在部分控制代码)
    for(int i = 0; i < 5; ++i) {
        int tempNum = this->getStrMemberNum(this->classAnswer[2][i]);
        if(tempNum != daynum) {
            if(tempNum > daynum) {

            } else {
                for(int k = 2; k < 4; ++k) {
                    for(int j = 0; j < this->memberNumber; ++j) {
                        if(this->isSubStr(this->classAnswer[k][i], this->member[j].getUsername())) {
                            continue;
                        }
                        if(this->member[j].getDay(i).getStr(k).length() == 0) {
                            this->classAnswer[k][i] += this->member[j].getUsername() + " ";
                        }
                        tempNum = this->getStrMemberNum(this->classAnswer[k][i]);
                        if(tempNum >= daynum) {
                            break;
                        }
                    }
                }
            }
        }
    }

    this->ui->comboBox->addItem("分配情况表");
    this->ui->comboBox->setCurrentIndex(this->memberNumber);
    this->MemberChanged(this->memberNumber);
}

void MainWindow::on_textEdit_1_textChanged()
{
    QString str = this->ui->textEdit_1->toPlainText();
    if(this->isDigitStr(str) == -1 || str.toInt() > 5) {
        this->ui->textEdit_1->setText("2");
    }
}

void MainWindow::on_textEdit_2_textChanged()
{
    QString str = this->ui->textEdit_2->toPlainText();
    if(this->isDigitStr(str) == -1 || str.toInt() > 7) {
        this->ui->textEdit_2->setText("2");
    }
}


