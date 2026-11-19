#include <QApplication>
#include <QDialog>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "mainwindow.h"

int main(int argc, char* argv[])
{
    // QApplication must be created first -- without it no Qt widgets work at all
    QApplication app(argc, argv);
    app.setApplicationName("Library Management System");
    app.setOrganizationName("OOP Project");

    // Show a login dialog before the main window opens.
    // If the user closes it or types wrong credentials, the program exits here.
    QDialog loginDialog;
    loginDialog.setWindowTitle("Library Management System — Login");
    loginDialog.setFixedSize(360, 265);
    loginDialog.setStyleSheet(
        "QDialog { background-color: #f8fafc; }"
        "QLabel  { color: #0f172a; }"
        "QLineEdit { color: #0f172a; background-color: white;"
        "  padding: 7px 10px; border: 1.5px solid #cbd5e1;"
        "  border-radius: 5px; font-size: 13px; }"
        "QLineEdit:focus { border: 1.5px solid #2563eb; }"
        "QPushButton { color: white; background-color: #2563eb;"
        "  border: none; border-radius: 5px;"
        "  padding: 8px 24px; font-size: 13px; }"
        "QPushButton:hover { background-color: #1d4ed8; }"
    );

    // QVBoxLayout stacks all widgets vertically inside the dialog
    QVBoxLayout *layout = new QVBoxLayout(&loginDialog);
    layout->setSpacing(10);
    layout->setContentsMargins(30, 24, 30, 20);

    QLabel *titleLabel = new QLabel("Library Management System");
    titleLabel->setStyleSheet("font-size: 15px; font-weight: bold; color: #1e293b;");
    titleLabel->setAlignment(Qt::AlignCenter);

    QLabel *subLabel = new QLabel("Enter your credentials to continue");
    subLabel->setStyleSheet("font-size: 12px; color: #64748b;");
    subLabel->setAlignment(Qt::AlignCenter);

    // QFormLayout gives us the two-column label + input layout
    QFormLayout *form = new QFormLayout;
    form->setSpacing(8);
    QLineEdit *userEdit = new QLineEdit;
    QLineEdit *passEdit = new QLineEdit;
    passEdit->setEchoMode(QLineEdit::Password);  // shows ●●●● instead of actual text
    userEdit->setPlaceholderText("Username");
    passEdit->setPlaceholderText("Password");
    form->addRow("Username:", userEdit);
    form->addRow("Password:", passEdit);

    // Error label starts hidden -- we only show it when login fails
    QLabel *errorLabel = new QLabel("Incorrect username or password.");
    errorLabel->setStyleSheet("color: #dc2626; font-size: 11px;");
    errorLabel->setAlignment(Qt::AlignCenter);
    errorLabel->hide();

    // Show the credentials on screen so the demo is easy to run
    QLabel *hintLabel = new QLabel("  Username: admin     Password: 1234");
    hintLabel->setStyleSheet(
        "color: #475569; font-size: 11px;"
        "background-color: #f1f5f9;"
        "border: 1px solid #e2e8f0;"
        "border-radius: 4px; padding: 5px 8px;");

    QPushButton *loginBtn = new QPushButton("Log In");

    layout->addWidget(titleLabel);
    layout->addWidget(subLabel);
    layout->addSpacing(4);
    layout->addWidget(hintLabel);
    layout->addSpacing(4);
    layout->addLayout(form);
    layout->addWidget(errorLabel);
    layout->addSpacing(6);
    layout->addWidget(loginBtn, 0, Qt::AlignRight);

    bool loggedIn = false;

    // Lambda slot: [&] captures all local variables by reference so we can
    // read userEdit, passEdit, errorLabel, and set loggedIn from inside it.
    QObject::connect(loginBtn, &QPushButton::clicked, [&]() {
        if (userEdit->text().trimmed() == "admin" && passEdit->text() == "1234") {
            loggedIn = true;
            loginDialog.accept();  // closes dialog and returns Accepted
        } else {
            errorLabel->show();
            passEdit->clear();     // clear password so user can retype
        }
    });

    // Pressing Enter in the password field triggers the login button
    QObject::connect(passEdit, &QLineEdit::returnPressed, loginBtn, &QPushButton::click);
    // Pressing Enter in the username field moves focus to the password field
    QObject::connect(userEdit, &QLineEdit::returnPressed, passEdit, static_cast<void(QLineEdit::*)()>(&QLineEdit::setFocus));

    // exec() blocks here until the dialog is closed.
    // If the user hit X or login failed, return 0 and the program ends.
    if (loginDialog.exec() != QDialog::Accepted || !loggedIn)
        return 0;

    // Login succeeded -- create and show the main window
    MainWindow window;
    window.show();
    return app.exec();  // starts the event loop -- runs until main window closes
}
