#include <qapplication.h>
#include <QDialog>
#include <QDialogButtonBox>
#include <QVBoxLayout>

#include <keditlistwidget.h>
#include <QDebug>

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps, true);


#if 0
    KEditListWidget::CustomEditor editor(new KComboBox(true, 0));
    KEditListWidget *box = new KEditListWidget(editor);

    box->insertItem(QStringLiteral("Test"));
    box->insertItem(QStringLiteral("for"));
    box->insertItem(QStringLiteral("this"));
    box->insertItem(QStringLiteral("KEditListWidget"));
    box->insertItem(QStringLiteral("Widget"));
    box->show();

#else

    // code from kexi
    QStringList list; list << QStringLiteral("one") << QStringLiteral("two");
    QDialog dialog;
    dialog.setObjectName(QStringLiteral("stringlist_dialog"));
    dialog.setModal(true);
    dialog.setWindowTitle(QStringLiteral("Edit List of Items"));

    KEditListWidget *edit = new KEditListWidget(&dialog);
    edit->setObjectName(QStringLiteral("editlist"));
    edit->insertStringList(list);

    QDialogButtonBox *buttonBox = new QDialogButtonBox(&dialog);
    buttonBox->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    QObject::connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(edit);
    layout->addWidget(buttonBox);
    dialog.setLayout(layout);

    if (dialog.exec() == QDialog::Accepted) {
        list = edit->items();
        qDebug() << list;
    }
#endif

    return app.exec();
}
