#pragma once

#include <QMainWindow>
#include "library.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using namespace std;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    Library *library;

    void buildMenuBar();
    void initTableHeaders();
    void loadSampleData();
    void applyStyle();

    void refreshBooks(const vector<shared_ptr<Book>> &list);
    void refreshAllBooks();
    void refreshUsers();
    void refreshBorrowTab();
    void refreshStats();
    void refreshUserDropdown();
    void showStatus(const QString &msg);

private slots:
    void onSearch();
    void onShowAll();
    void onAddBook();
    void onRemoveBook();

    void onUserChanged(int index);
    void onBorrow();
    void onReturn();

    void onAddUser();
    void onRemoveUser();

    void onSave();
    void onLoad();
    void onAbout();
    void onTabChanged(int index);
};
