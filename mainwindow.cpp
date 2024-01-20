#include "mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    text1 = new QTextEdit;
    this->setCentralWidget(text1);

    QFont f;
    f.setPixelSize(30);
    text1->setFont(f);


    wenjian = this->menuBar()->addMenu("文件");
    bianji = this->menuBar()->addMenu("编辑");
    bushu = this->menuBar()->addMenu("构建");
    help = this->menuBar()->addMenu("帮助");


    wenjian_open = new QAction("打开", this);
    wenjian_open->setShortcut(tr("Ctrl+O"));

    wenjian_save = new QAction("保存", this);
    wenjian_save->setShortcut(tr("Ctrl+S"));

    wenjian_exit = new QAction("退出", this);
    wenjian->addAction(wenjian_open);
    wenjian->addAction(wenjian_save);
    wenjian->addSeparator();
    wenjian->addAction(wenjian_exit);

    bushu_compile = new QAction("编译", this);
    bushu->addAction(bushu_compile);
    bushu_run = new QAction("运行", this);
    bushu->addAction(bushu_run);

    help_about = new QAction("关于", this);
    help->addAction(help_about);
    help_about->setShortcut((tr("Ctrl+H")));

    bianji_copy = new QAction("拷贝", this);
    bianji->addAction(bianji_copy);

    bianji_paste = new QAction("粘贴", this);
    bianji->addAction(bianji_paste);

    bianji_cut = new QAction("剪切", this);
    bianji->addAction(bianji_cut);

    bianji_selectAll = new QAction("全选", this);
    bianji->addAction(bianji_selectAll);




    connect(wenjian_open, SIGNAL(triggered()), this, SLOT(on_open()));
    connect(wenjian_save, SIGNAL(triggered()), this, SLOT(on_save()));

    connect(help_about, SIGNAL(triggered()), this, SLOT(on_about()));
    connect(wenjian_exit, SIGNAL(triggered()), this, SLOT(on_exit()));
    connect(bianji_copy, SIGNAL(triggered()), this, SLOT(on_copy()));
    connect(bianji_paste, SIGNAL(triggered()), this, SLOT(on_paste()));
    connect(bianji_cut, SIGNAL(triggered()), this, SLOT(on_cut()));
    connect(bianji_selectAll, SIGNAL(triggered()), this, SLOT(on_selectAll()));
    connect(bushu_compile, SIGNAL(triggered()), this, SLOT(on_compile()));
    connect(bushu_run, SIGNAL(triggered()), this, SLOT(on_run()));
}

MainWindow::~MainWindow()
{
    delete text1;
}

void MainWindow::on_open()
{


    wenjianname = QFileDialog::getOpenFileName();
    if (wenjianname.isEmpty())
        return;
    QString content;
    FILE *p = fopen(wenjianname.toStdString().data(), "r");
    if (p == NULL)
    {
        QMessageBox::information(this, "错误", "打开文件失败");
    }
    else
    {
        while (!feof(p))
        {
            char buf[1024];
            fgets(buf, sizeof(buf), p);
            content += buf;
        }
        fclose(p);
        text1->setText(content);
    }

}

void MainWindow::on_save()
{
    wenjianname = QFileDialog::getSaveFileName();
    if (wenjianname.isEmpty())
        return;
    FILE *p = fopen(wenjianname.toStdString().data(), "w");
    if (p == NULL)
    {
        QMessageBox::information(this, "错误", "打开文件失败");
    }
    else
    {
        fputs(text1->toPlainText().toStdString().data(), p);
        fclose(p);
    }

}

void MainWindow::on_about()
{
    QMessageBox::information(this, "关于", "槿华jhhua：版权所有\n2016-2018");
}

void MainWindow::on_exit()
{
    exit(0);
}

void MainWindow::on_copy()
{
    text1->copy();
}

void MainWindow::on_paste()
{
    text1->paste();
}

void MainWindow::on_cut()
{
    text1->cut();
}

void MainWindow::on_selectAll()
{
    text1->selectAll();
}

void MainWindow::on_compile()
{
    QString content = "#include <stdio.h>\n";
    FILE *p = fopen(wenjianname.toStdString().data(), "r");
    if (p == NULL)
        return;
    while (!feof(p))
    {
        char buf[1024] = { 0 };
        fgets(buf, sizeof(buf), p);
        content += buf;
    }
    fclose(p);
    content.replace("整数", "int").replace("主函数", "main").replace("开始", "{").replace("打印", "printf(\"hello world\")").replace("返回", "return").replace("结束", "}").replace("等待", "getchar()");


    QString destname = wenjianname;
    destname.replace(".e", ".c");
    p = fopen(destname.toStdString().data(), "w");
    fputs(content.toStdString().data(), p);
    fclose(p);


    QString srcname = destname;
    srcname.replace(".c", ".exe");
    QString command = "gcc -o " + srcname + " " + destname;
    system(command.toStdString().data());
    remove(destname.toStdString().data());
}

void MainWindow::on_run()
{
    QString destname = wenjianname;
    destname.replace(".e", ".exe");
    system(destname.toStdString().data());
}
