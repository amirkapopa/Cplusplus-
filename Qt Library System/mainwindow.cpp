#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "exceptions.h"
#include "filehandler.h"

#include <QApplication>
#include <QPalette>
#include <QHeaderView>
#include <QMenuBar>
#include <QMenu>
#include <QStatusBar>
#include <QMessageBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QSpinBox>
#include <QGridLayout>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    library = new Library("City Library");

    // macOS Dark Mode makes all text white by default, which looks terrible on
    // our light-coloured UI. Setting the palette manually before setupUi()
    // fixes it. We spent a while debugging this -- the order matters a lot.
    QPalette pal = qApp->palette();
    pal.setColor(QPalette::Window,          QColor(248, 250, 252));
    pal.setColor(QPalette::WindowText,      QColor( 15,  23,  42));
    pal.setColor(QPalette::Base,            QColor(255, 255, 255));
    pal.setColor(QPalette::AlternateBase,   QColor(241, 245, 249));
    pal.setColor(QPalette::Text,            QColor( 15,  23,  42));
    pal.setColor(QPalette::ButtonText,      QColor( 15,  23,  42));
    pal.setColor(QPalette::Highlight,       QColor(219, 234, 254));
    pal.setColor(QPalette::HighlightedText, QColor( 15,  23,  42));
    qApp->setPalette(pal);

    ui->setupUi(this);

    initTableHeaders();
    buildMenuBar();
    applyStyle();
    loadSampleData();

    // Connect each button/widget signal to its slot function.
    // Qt's signal-slot mechanism is what replaces raw callback pointers.
    connect(ui->tabs,         &QTabWidget::currentChanged,
            this,             &MainWindow::onTabChanged);

    connect(ui->searchBtn,    &QPushButton::clicked,         this, &MainWindow::onSearch);
    connect(ui->showAllBtn,   &QPushButton::clicked,         this, &MainWindow::onShowAll);
    connect(ui->addBookBtn,   &QPushButton::clicked,         this, &MainWindow::onAddBook);
    connect(ui->removeBookBtn,&QPushButton::clicked,         this, &MainWindow::onRemoveBook);
    connect(ui->searchBox,    &QLineEdit::returnPressed,     this, &MainWindow::onSearch);

    connect(ui->userSelector, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,             &MainWindow::onUserChanged);
    connect(ui->borrowBtn,        &QPushButton::clicked,        this, &MainWindow::onBorrow);
    connect(ui->returnBtn,        &QPushButton::clicked,        this, &MainWindow::onReturn);
    connect(ui->availableTable,   &QTableWidget::doubleClicked, this, &MainWindow::onBorrow);
    connect(ui->borrowedTable,    &QTableWidget::doubleClicked, this, &MainWindow::onReturn);
    // double-click on either borrow/return table does the same as pressing the button

    connect(ui->addUserBtn,    &QPushButton::clicked, this, &MainWindow::onAddUser);
    connect(ui->removeUserBtn, &QPushButton::clicked, this, &MainWindow::onRemoveUser);

    refreshAllBooks();
    refreshUsers();
    refreshUserDropdown();
    refreshStats();

    statusBar()->showMessage("Ready  —  City Public Library");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete library;
}

void MainWindow::buildMenuBar()
{
    QMenu *fileMenu = menuBar()->addMenu("  File  ");
    fileMenu->addAction("Save Data",  this, &MainWindow::onSave);
    fileMenu->addAction("Load Data",  this, &MainWindow::onLoad);
    fileMenu->addSeparator();
    fileMenu->addAction("Exit",       this, &QWidget::close);

    QMenu *helpMenu = menuBar()->addMenu("  Help  ");
    helpMenu->addAction("About",      this, &MainWindow::onAbout);
}

void MainWindow::initTableHeaders()
{
    ui->booksTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->booksTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->booksTable->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    ui->booksTable->verticalHeader()->setDefaultSectionSize(40);

    ui->availableTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->availableTable->verticalHeader()->setDefaultSectionSize(38);

    ui->borrowedTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->borrowedTable->verticalHeader()->setDefaultSectionSize(38);

    ui->usersTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->usersTable->verticalHeader()->setDefaultSectionSize(40);

    QGridLayout *statsGrid = ui->statsTab->findChild<QGridLayout*>("statsGrid");
    if (statsGrid)
    {
        statsGrid->setColumnStretch(0, 1);
        statsGrid->setColumnStretch(1, 1);
    }
}

void MainWindow::applyStyle()
{
    setStyleSheet(

        "QWidget { color: #0f172a; font-size: 13px; }"
        "QMainWindow { background-color: #f8fafc; }"

        "QWidget#appHeader  { background-color: #1e293b; }"
        "QLabel#appTitle    { color: #f1f5f9; font-size: 15px; font-weight: bold;"
        "                     background-color: transparent; }"
        "QLabel#appDivider  { color: #475569; background-color: transparent; }"
        "QLabel#appSubtitle { color: #94a3b8; font-size: 13px;"
        "                     background-color: transparent; }"
        "QLabel#headerBadge { color: #93c5fd; font-size: 11px;"
        "                     background-color: #334155;"
        "                     border: 1px solid #475569;"
        "                     border-radius: 4px; padding: 3px 10px; }"

        "QMenuBar            { color: #cbd5e1; background-color: #1e293b; font-size: 13px; }"
        "QMenuBar::item      { color: #cbd5e1; background-color: transparent; padding: 6px 14px; }"
        "QMenuBar::item:selected { color: white; background-color: #334155; }"
        "QMenu               { color: #0f172a; background-color: white; border: 1px solid #e2e8f0; }"
        "QMenu::item         { color: #0f172a; padding: 7px 22px; }"
        "QMenu::item:selected{ color: #1d4ed8; background-color: #dbeafe; }"

        "QTabWidget::pane    { background-color: #f8fafc; border: none;"
        "                      border-top: 1px solid #e2e8f0; }"
        "QTabBar             { background-color: white; }"
        "QTabBar::tab        { color: #64748b; background-color: transparent;"
        "                      padding: 12px 28px; font-size: 13px;"
        "                      border: none; border-bottom: 3px solid transparent; }"
        "QTabBar::tab:selected   { color: #2563eb; font-weight: bold;"
        "                          border-bottom: 3px solid #2563eb; }"
        "QTabBar::tab:hover:!selected { color: #1e40af; background-color: #eff6ff; }"

        "QGroupBox { color: #374151; font-weight: bold; font-size: 12px;"
        "            background-color: white;"
        "            border: 1px solid #e2e8f0; border-radius: 6px;"
        "            margin-top: 14px; padding-top: 10px; }"
        "QGroupBox::title { color: #374151; subcontrol-origin: margin;"
        "                   subcontrol-position: top left; padding: 0 8px; }"
        "QGroupBox QLabel   { color: #0f172a; background-color: transparent; }"

        "QFrame#statBooksCard, QFrame#statAvailableCard,"
        "QFrame#statUsersCard, QFrame#statBorrowsCard {"
        "    background-color: white;"
        "    border-top:    1px solid #e2e8f0;"
        "    border-right:  1px solid #e2e8f0;"
        "    border-bottom: 1px solid #e2e8f0;"
        "    border-left:   4px solid #2563eb;"
        "    border-radius: 6px; }"

        "QLabel#statBooks, QLabel#statAvailable,"
        "QLabel#statUsers, QLabel#statBorrows {"
        "    color: #2563eb; font-size: 40px; font-weight: bold;"
        "    background-color: transparent; }"

        "QLabel#statBooksDesc, QLabel#statAvailableDesc,"
        "QLabel#statUsersDesc, QLabel#statBorrowsDesc {"
        "    color: #64748b; font-size: 12px; background-color: transparent; }"

        "QLabel#dashHeading { color: #0f172a; font-size: 18px; font-weight: bold;"
        "                     background-color: transparent; }"
        "QLabel#conceptLabel       { color: #374151; background-color: transparent; }"
        "QLabel#statBookObjects    { color: #2563eb; font-size: 16px; font-weight: bold;"
        "                            background-color: transparent; }"
        "QLabel#conceptExplanation { color: #475569; font-size: 12px;"
        "                            background-color: transparent; }"

        "QTableWidget { color: #0f172a; background-color: white;"
        "               border: 1px solid #e2e8f0; border-radius: 4px;"
        "               gridline-color: transparent; }"
        "QTableWidget::item           { color: #0f172a; background-color: white;"
        "                               padding: 6px 10px; border: none; }"
        "QTableWidget::item:alternate { color: #0f172a; background-color: #f8fafc; }"
        "QTableWidget::item:selected  { color: #1d4ed8; background-color: #dbeafe; }"
        "QHeaderView::section { color: #475569; background-color: #f1f5f9;"
        "                       font-size: 11px; font-weight: bold;"
        "                       padding: 10px 10px; border: none;"
        "                       border-bottom: 2px solid #e2e8f0;"
        "                       border-right: 1px solid #e2e8f0; }"
        "QHeaderView::section:last { border-right: none; }"

        "QLineEdit { color: #0f172a; background-color: white;"
        "            padding: 7px 10px; border: 1.5px solid #cbd5e1;"
        "            border-radius: 5px; font-size: 13px; }"
        "QLineEdit:focus { border: 1.5px solid #2563eb; }"

        "QComboBox { color: #0f172a; background-color: white;"
        "            padding: 7px 10px; border: 1.5px solid #cbd5e1;"
        "            border-radius: 5px; font-size: 13px; }"
        "QComboBox:focus { border: 1.5px solid #2563eb; }"
        "QComboBox::drop-down { border: none; width: 20px; }"
        "QComboBox QAbstractItemView { color: #0f172a; background-color: white;"
        "    border: 1.5px solid #cbd5e1; border-radius: 4px;"
        "    selection-color: #1d4ed8; selection-background-color: #dbeafe;"
        "    outline: none; }"

        "QSpinBox { color: #0f172a; background-color: white;"
        "           border: 1.5px solid #cbd5e1; border-radius: 5px; padding: 7px 8px; }"
        "QSpinBox:focus { border: 1.5px solid #2563eb; }"

        "QPushButton { color: white; background-color: #2563eb;"
        "              border: none; border-radius: 5px;"
        "              padding: 8px 20px; font-size: 13px;"
        "              font-weight: 500; min-width: 90px; }"
        "QPushButton:hover   { background-color: #1d4ed8; }"
        "QPushButton:pressed { background-color: #1e40af; }"

        "QPushButton#showAllBtn, QPushButton#returnBtn {"
        "    color: #2563eb; background-color: #eff6ff; border: 1.5px solid #bfdbfe; }"
        "QPushButton#showAllBtn:hover, QPushButton#returnBtn:hover"
        "    { background-color: #dbeafe; }"
        "QPushButton#showAllBtn:pressed, QPushButton#returnBtn:pressed"
        "    { background-color: #bfdbfe; }"

        "QPushButton#removeBookBtn, QPushButton#removeUserBtn {"
        "    color: white; background-color: #dc2626; }"
        "QPushButton#removeBookBtn:hover, QPushButton#removeUserBtn:hover"
        "    { background-color: #b91c1c; }"
        "QPushButton#removeBookBtn:pressed, QPushButton#removeUserBtn:pressed"
        "    { background-color: #991b1b; }"

        "QLabel { color: #0f172a; background-color: transparent; }"
        "QLabel#searchLabel    { color: #374151; background-color: transparent; }"
        "QLabel#bookCountLabel { color: #64748b; font-size: 12px;"
        "                        background-color: transparent; }"
        "QLabel#sectionInfoLabel { color: #64748b; font-size: 12px;"
        "                          background-color: transparent; }"
        "QLabel#borrowInfoLabel { color: #374151; font-weight: bold;"
        "                         background-color: transparent; }"

        "QStatusBar { color: #64748b; background-color: #f1f5f9;"
        "             border-top: 1px solid #e2e8f0; font-size: 12px; }"

        "QDialog { background-color: #f8fafc; }"
        "QDialog QLabel { color: #0f172a; background-color: transparent; }"
    );
}

void MainWindow::loadSampleData()
{
    library->addBook(make_shared<FictionBook>(
        "The Great Gatsby",                     "F. Scott Fitzgerald", "978-0-7432-7356-5", 1925, "Classic"));
    library->addBook(make_shared<FictionBook>(
        "To Kill a Mockingbird",                "Harper Lee",          "978-0-06-112008-4", 1960, "Drama"));
    library->addBook(make_shared<FictionBook>(
        "Harry Potter and the Sorcerers Stone", "J.K. Rowling",        "978-0-590-35340-3", 1997, "Fantasy"));
    library->addBook(make_shared<FictionBook>(
        "The Hobbit",                           "J.R.R. Tolkien",      "978-0-547-92822-7", 1937, "Fantasy"));
    library->addBook(make_shared<FictionBook>(
        "1984",                                 "George Orwell",       "978-0-451-52493-5", 1949, "Dystopian"));

    library->addBook(make_shared<NonFictionBook>(
        "A Brief History of Time",  "Stephen Hawking",     "978-0-553-38016-3", 1988, "Physics"));
    library->addBook(make_shared<NonFictionBook>(
        "Sapiens",                  "Yuval Noah Harari",   "978-0-062-31611-3", 2011, "History"));
    library->addBook(make_shared<NonFictionBook>(
        "The Selfish Gene",         "Richard Dawkins",     "978-0-198-57519-1", 1976, "Biology"));
    library->addBook(make_shared<NonFictionBook>(
        "Thinking Fast and Slow",   "Daniel Kahneman",     "978-0-374-27563-1", 2011, "Psychology"));
    library->addBook(make_shared<NonFictionBook>(
        "Clean Code",               "Robert C. Martin",    "978-0-132-35088-4", 2008, "Programming"));

    library->addUser(make_shared<User>("Alice Johnson", "U001", 3));
    library->addUser(make_shared<User>("Bob Smith",     "U002", 2));
    library->addUser(make_shared<User>("Carol White",   "U003", 3));
}

// ---- Refresh methods ----
// These functions re-read from the library object and rebuild the table from scratch.
// It's simpler than trying to update individual rows in-place.

void MainWindow::refreshBooks(const vector<shared_ptr<Book>> &list)
{
    ui->booksTable->setRowCount(0);

    for (const auto &book : list)
    {
        int row = ui->booksTable->rowCount();
        ui->booksTable->insertRow(row);

        ui->booksTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(book->getTitle())));
        ui->booksTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(book->getAuthor())));
        ui->booksTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(book->getISBN())));
        ui->booksTable->setItem(row, 3, new QTableWidgetItem(QString::number(book->getYear())));
        // getType() and getDescription() are virtual -- the correct override is
        // picked at runtime based on whether the book is Fiction or NonFiction
        ui->booksTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(book->getType())));
        ui->booksTable->setItem(row, 5, new QTableWidgetItem(QString::fromStdString(book->getDescription())));

        QString statusText  = book->isAvailable() ? "Available" : "Borrowed";
        QColor  statusColor = book->isAvailable() ? QColor(22, 163, 74) : QColor(220, 38, 38);
        QTableWidgetItem *statusItem = new QTableWidgetItem(statusText);
        statusItem->setForeground(statusColor);
        ui->booksTable->setItem(row, 6, statusItem);

        ui->booksTable->item(row, 0)->setData(Qt::UserRole, QString::fromStdString(book->getISBN()));
    }

    ui->bookCountLabel->setText(QString("Showing %1 book(s)").arg(list.size()));
}

void MainWindow::refreshAllBooks()
{
    refreshBooks(library->getBooks());
}

void MainWindow::refreshUsers()
{
    ui->usersTable->setRowCount(0);

    for (const auto &user : library->getUsers())
    {
        int row = ui->usersTable->rowCount();
        ui->usersTable->insertRow(row);

        ui->usersTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(user->getPersonId())));
        ui->usersTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(user->getName())));
        ui->usersTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(user->getRole())));
        ui->usersTable->setItem(row, 3, new QTableWidgetItem(QString::number(user->getBorrowedCount())));
        ui->usersTable->setItem(row, 4, new QTableWidgetItem(QString::number(user->getMaxBorrowLimit())));

        ui->usersTable->item(row, 0)->setData(Qt::UserRole, QString::fromStdString(user->getPersonId()));
    }
}

void MainWindow::refreshUserDropdown()
{
    QString currentId = ui->userSelector->currentData().toString();
    ui->userSelector->clear();

    for (const auto &user : library->getUsers())
    {
        QString displayText = QString::fromStdString(user->getName() + "  (" + user->getPersonId() + ")");
        QString idData      = QString::fromStdString(user->getPersonId());
        ui->userSelector->addItem(displayText, idData);
    }

    int idx = ui->userSelector->findData(currentId);
    if (idx >= 0)
        ui->userSelector->setCurrentIndex(idx);
}

void MainWindow::refreshBorrowTab()
{
    ui->availableTable->setRowCount(0);

    for (const auto &book : library->getBooks())
    {
        if (!book->isAvailable()) continue;

        int row = ui->availableTable->rowCount();
        ui->availableTable->insertRow(row);
        ui->availableTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(book->getTitle())));
        ui->availableTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(book->getAuthor())));
        ui->availableTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(book->getISBN())));
        ui->availableTable->setItem(row, 3, new QTableWidgetItem(QString::number(book->getYear())));
        ui->availableTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(book->getType())));
        ui->availableTable->item(row, 0)->setData(Qt::UserRole, QString::fromStdString(book->getISBN()));
    }

    ui->borrowedTable->setRowCount(0);

    if (ui->userSelector->currentIndex() < 0) return;

    QString userId = ui->userSelector->currentData().toString();

    try
    {
        auto user = library->findUser(userId.toStdString());

        for (const auto &isbn : user->getBorrowedISBNs())
        {
            try
            {
                auto book = library->searchByISBN(isbn);
                int row = ui->borrowedTable->rowCount();
                ui->borrowedTable->insertRow(row);
                ui->borrowedTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(book->getTitle())));
                ui->borrowedTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(book->getAuthor())));
                ui->borrowedTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(book->getISBN())));
                ui->borrowedTable->setItem(row, 3, new QTableWidgetItem(QString::number(book->getYear())));
                ui->borrowedTable->item(row, 0)->setData(Qt::UserRole, QString::fromStdString(isbn));
            }
            catch (...) {}
        }

        ui->borrowInfoLabel->setText(
            QString("Borrowed:  %1 / %2")
                .arg(user->getBorrowedCount())
                .arg(user->getMaxBorrowLimit()));
    }
    catch (...)
    {
        ui->borrowInfoLabel->setText("Member not found");
    }
}

void MainWindow::refreshStats()
{
    ui->statBooks->setText(    QString::number(library->getBooks().size()));
    ui->statAvailable->setText(QString::number(library->countAvailableBooks()));
    ui->statUsers->setText(    QString::number(library->getUsers().size()));
    ui->statBorrows->setText(  QString::number(Library::getTotalBorrowsAllTime()));

    // Book::getBookCount() is a static method -- called on the class itself, no object needed.
    // This number changes live as books are added or removed.
    ui->statBookObjects->setText(QString::number(Book::getBookCount()));
}

void MainWindow::showStatus(const QString &msg)
{
    statusBar()->showMessage(msg, 4000);
}

void MainWindow::onTabChanged(int index)
{
    if      (index == 0) refreshAllBooks();
    else if (index == 1) refreshBorrowTab();
    else if (index == 2) refreshUsers();
    else if (index == 3) refreshStats();
}

// ---- Slots: Books tab ----

// onSearch() uses three different backend search methods depending on the dropdown.
// All three have the same concept (search by something) but different signatures --
// that's function overloading / compile-time polymorphism.
void MainWindow::onSearch()
{
    QString query = ui->searchBox->text().trimmed();
    if (query.isEmpty()) { onShowAll(); return; }

    string q    = query.toStdString();
    string type = ui->searchType->currentText().toStdString();

    vector<shared_ptr<Book>> results;

    try
    {
        if      (type == "Title")  results = library->searchByTitle(q);
        else if (type == "Author") results = library->searchByAuthor(q);
        else
        {
            results.push_back(library->searchByISBN(q));
        }
    }
    catch (const BookNotFoundException &)
    {
        // ISBN search throws if nothing found -- just leave the table empty
    }
    catch (const LibraryException &e)
    {
        QMessageBox::warning(this, "Search Error", e.what());
        return;
    }

    refreshBooks(results);
    showStatus(QString("Found %1 result(s) for \"%2\"").arg(results.size()).arg(query));
}

void MainWindow::onShowAll()
{
    ui->searchBox->clear();
    refreshAllBooks();
    showStatus("Showing all books.");
}

void MainWindow::onAddBook()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Add New Book");
    dialog.setMinimumWidth(420);

    QFormLayout *form = new QFormLayout(&dialog);
    form->setSpacing(10);
    form->setContentsMargins(18, 18, 18, 14);

    QLineEdit *titleEdit  = new QLineEdit;
    QLineEdit *authorEdit = new QLineEdit;
    QLineEdit *isbnEdit   = new QLineEdit;
    QSpinBox  *yearSpin   = new QSpinBox;
    yearSpin->setRange(1000, 2100);
    yearSpin->setValue(2024);

    QComboBox *typeCombo = new QComboBox;
    typeCombo->addItems({"Fiction", "Non-Fiction"});

    QLineEdit *extraEdit = new QLineEdit;
    extraEdit->setPlaceholderText("Fiction → genre (e.g. Fantasy)    Non-Fiction → subject (e.g. History)");

    form->addRow("Title:",           titleEdit);
    form->addRow("Author:",          authorEdit);
    form->addRow("ISBN:",            isbnEdit);
    form->addRow("Year:",            yearSpin);
    form->addRow("Type:",            typeCombo);
    form->addRow("Genre / Subject:", extraEdit);

    QDialogButtonBox *buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    form->addRow(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) return;

    try
    {
        string title  = titleEdit->text().trimmed().toStdString();
        string author = authorEdit->text().trimmed().toStdString();
        string isbn   = isbnEdit->text().trimmed().toStdString();
        int    year   = yearSpin->value();
        string extra  = extraEdit->text().trimmed().toStdString();

        if (title.empty())  throw InvalidDataException("Title cannot be empty.");
        if (author.empty()) throw InvalidDataException("Author cannot be empty.");
        if (isbn.empty())   throw InvalidDataException("ISBN cannot be empty.");
        if (extra.empty())  extra = (typeCombo->currentIndex() == 0) ? "General Fiction" : "General";

        shared_ptr<Book> newBook;
        if (typeCombo->currentIndex() == 0)
            newBook = make_shared<FictionBook>(title, author, isbn, year, extra);
        else
            newBook = make_shared<NonFictionBook>(title, author, isbn, year, extra);

        library->addBook(newBook);
        refreshAllBooks();
        refreshStats();
        showStatus("Book added: " + QString::fromStdString(title));
    }
    catch (const LibraryException &e)
    {
        QMessageBox::warning(this, "Cannot Add Book", e.what());
    }
}

void MainWindow::onRemoveBook()
{
    int row = ui->booksTable->currentRow();
    if (row < 0)
    {
        QMessageBox::information(this, "No Selection", "Please select a book from the table first.");
        return;
    }

    QString isbn  = ui->booksTable->item(row, 0)->data(Qt::UserRole).toString();
    QString title = ui->booksTable->item(row, 0)->text();

    if (QMessageBox::question(this, "Remove Book",
            "Remove \"" + title + "\" from the catalogue?",
            QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
        return;

    try
    {
        library->removeBook(isbn.toStdString());
        refreshAllBooks();
        refreshStats();
        showStatus("Removed: " + title);
    }
    catch (const LibraryException &e)
    {
        QMessageBox::warning(this, "Error", e.what());
    }
}

// ---- Slots: Borrow / Return tab ----

void MainWindow::onUserChanged(int /*index*/)
{
    refreshBorrowTab();
}

void MainWindow::onBorrow()
{
    if (ui->userSelector->currentIndex() < 0)
    {
        QMessageBox::information(this, "No Member", "Please select a member first.");
        return;
    }
    int row = ui->availableTable->currentRow();
    if (row < 0)
    {
        QMessageBox::information(this, "No Book", "Please select a book to borrow.");
        return;
    }

    QString userId = ui->userSelector->currentData().toString();
    QString isbn   = ui->availableTable->item(row, 0)->data(Qt::UserRole).toString();
    QString title  = ui->availableTable->item(row, 0)->text();

    try
    {
        library->borrowBook(userId.toStdString(), isbn.toStdString());
        refreshBorrowTab();
        refreshStats();
        showStatus("Borrowed: " + title);
    }
    catch (const LibraryException &e)
    {
        QMessageBox::warning(this, "Cannot Borrow", e.what());
    }
}

void MainWindow::onReturn()
{
    if (ui->userSelector->currentIndex() < 0) return;

    int row = ui->borrowedTable->currentRow();
    if (row < 0)
    {
        QMessageBox::information(this, "No Book", "Please select a book to return.");
        return;
    }

    QString userId = ui->userSelector->currentData().toString();
    QString isbn   = ui->borrowedTable->item(row, 0)->data(Qt::UserRole).toString();
    QString title  = ui->borrowedTable->item(row, 0)->text();

    try
    {
        library->returnBook(userId.toStdString(), isbn.toStdString());
        refreshBorrowTab();
        refreshStats();
        showStatus("Returned: " + title);
    }
    catch (const LibraryException &e)
    {
        QMessageBox::warning(this, "Cannot Return", e.what());
    }
}

// ---- Slots: Members tab ----

void MainWindow::onAddUser()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Add New Member");
    dialog.setMinimumWidth(340);

    QFormLayout *form = new QFormLayout(&dialog);
    form->setSpacing(10);
    form->setContentsMargins(18, 18, 18, 14);

    QLineEdit *nameEdit  = new QLineEdit;
    QLineEdit *idEdit    = new QLineEdit;
    QSpinBox  *limitSpin = new QSpinBox;
    limitSpin->setRange(1, 10);
    limitSpin->setValue(3);

    form->addRow("Full Name:",           nameEdit);
    form->addRow("Member ID:",           idEdit);
    form->addRow("Max books to borrow:", limitSpin);

    QDialogButtonBox *buttons = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    form->addRow(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) return;

    try
    {
        string name = nameEdit->text().trimmed().toStdString();
        string id   = idEdit->text().trimmed().toStdString();

        if (name.empty()) throw InvalidDataException("Name cannot be empty.");
        if (id.empty())   throw InvalidDataException("Member ID cannot be empty.");

        library->addUser(make_shared<User>(name, id, limitSpin->value()));
        refreshUsers();
        refreshUserDropdown();
        refreshStats();
        showStatus("Member added: " + QString::fromStdString(name));
    }
    catch (const LibraryException &e)
    {
        QMessageBox::warning(this, "Cannot Add Member", e.what());
    }
}

void MainWindow::onRemoveUser()
{
    int row = ui->usersTable->currentRow();
    if (row < 0)
    {
        QMessageBox::information(this, "No Selection", "Please select a member first.");
        return;
    }

    QString userId = ui->usersTable->item(row, 0)->data(Qt::UserRole).toString();
    QString name   = ui->usersTable->item(row, 1)->text();

    if (QMessageBox::question(this, "Remove Member",
            "Remove member \"" + name + "\"?",
            QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
        return;

    try
    {
        library->removeUser(userId.toStdString());
        refreshUsers();
        refreshUserDropdown();
        refreshBorrowTab();
        refreshStats();
        showStatus("Removed member: " + name);
    }
    catch (const LibraryException &e)
    {
        QMessageBox::warning(this, "Error", e.what());
    }
}

// ---- Slots: Menu bar ----

void MainWindow::onSave()
{
    try
    {
        FileHandler::saveAll(library->getBooks(), library->getUsers());
        QMessageBox::information(this, "Saved",
            "Data saved to:\n  books.csv\n  users.csv");
        showStatus("Data saved.");
    }
    catch (const LibraryException &e)
    {
        QMessageBox::critical(this, "Save Failed", e.what());
    }
}

void MainWindow::onLoad()
{
    if (QMessageBox::question(this, "Load Data",
            "This will replace all current data with the saved CSV files. Continue?",
            QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes)
        return;

    try
    {
        auto loadedBooks = FileHandler::loadBooks();
        auto loadedUsers = FileHandler::loadUsers();

        // Delete the old library and make a fresh one, then repopulate it.
        // Easier than trying to clear each vector and reset stats manually.
        delete library;
        library = new Library("City Library");

        for (auto &b : loadedBooks) library->addBook(b);
        for (auto &u : loadedUsers) library->addUser(u);

        refreshAllBooks();
        refreshUsers();
        refreshUserDropdown();
        refreshBorrowTab();
        refreshStats();
        showStatus("Data loaded from CSV files.");
    }
    catch (const LibraryException &e)
    {
        QMessageBox::critical(this, "Load Failed", e.what());
    }
}

void MainWindow::onAbout()
{
    QMessageBox::about(this, "About — Library Management System",
        "Library Management System\n"
        "A C++ OOP project built with Qt 5\n\n"
        "OOP Concepts Demonstrated:\n"
        "  Classes & Objects\n"
        "  Constructors & Destructors\n"
        "  Function Overloading  (searchByTitle / Author / ISBN)\n"
        "  Operator Overloading  (==, >, << on Book)\n"
        "  Runtime Polymorphism  (virtual getType(), getDescription())\n"
        "  Compile-time Polymorphism (overloaded functions)\n"
        "  Single Inheritance    (FictionBook : Book)\n"
        "  Hierarchical Inheritance (FictionBook, NonFictionBook : Book)\n"
        "  Multiple Inheritance  (User : Person, IMember)\n"
        "  Static Members        (Book::bookCount)\n"
        "  File Handling         (books.csv, users.csv)\n"
        "  Exception Handling    (custom LibraryException hierarchy)\n\n"
        "First-year university project.");
}
