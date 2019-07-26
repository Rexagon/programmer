/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include "MainWindowUI.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QVBoxLayout>

namespace
{
constexpr auto WINDOW_WIDTH_MIN = 900;
constexpr auto WINDOW_HEIGHT_MIN = 600;
} // namespace


namespace app
{
MainWindowUI::MainWindowUI(QMainWindow *window)
    : m_window{window}
{
    m_window->setMinimumSize(WINDOW_WIDTH_MIN, WINDOW_HEIGHT_MIN);

    auto *container = new QWidget(m_window);
    auto *layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(createTopWorkspace());
    layout->addWidget(createBottomWorkspace());

    m_window->setCentralWidget(container);
}


SerialPortSelector *MainWindowUI::getSerialPortSelector() const
{
    return m_serialPortSelector;
}


BaudRateSelector *MainWindowUI::getBaudRateSelector() const
{
    return m_baudRateSelector;
}


QProgressBar *MainWindowUI::getProgressBar() const
{
    return m_progressBar;
}


QPushButton *MainWindowUI::getVerifyButton() const
{
    return m_verifyButton;
}


QPushButton *MainWindowUI::getWriteButton() const
{
    return m_writeButton;
}


QFileDialog *MainWindowUI::getFileDialog() const
{
    return m_fileDialog;
}


QPushButton *MainWindowUI::getFileSelectionButton() const
{
    return m_fileSelectionButton;
}


void MainWindowUI::setSelectedFilePath(const QString &path)
{
    m_selectedFileLabel->setText(path);
}


QWidget *MainWindowUI::createTopWorkspace()
{
    auto *container = new QWidget(m_window);
    auto *layout = new QHBoxLayout(container);

    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Left side
    auto *leftContainer = new QWidget(m_window);
    auto *leftLayout = new QVBoxLayout(leftContainer);

    // Serial port selector
    QLabel *serialPortLabel = new QLabel("Порт:", m_window);
    leftLayout->addWidget(serialPortLabel);

    m_serialPortSelector = new SerialPortSelector(m_window);
    leftLayout->addWidget(m_serialPortSelector);

    // Baud rate selector
    QLabel *baudRateLabel = new QLabel("BaudRate:", m_window);
    leftLayout->addWidget(baudRateLabel);

    m_baudRateSelector = new BaudRateSelector(m_window);
    leftLayout->addWidget(m_baudRateSelector);

    //
    leftLayout->addStretch();

    layout->addWidget(leftContainer);

    // Right side
    auto *rightContainer = new QWidget(m_window);
    /*auto *rightLayout = */ new QVBoxLayout(rightContainer);

    layout->addWidget(rightContainer);

    //
    return container;
}


QWidget *MainWindowUI::createBottomWorkspace()
{
    auto *container = new QWidget(m_window);
    auto *layout = new QVBoxLayout(container);

    // File selection widgets group
    auto *fileSelectionContainer = new QWidget(m_window);
    auto *fileSelectionLayout = new QHBoxLayout(fileSelectionContainer);

    m_fileDialog = new QFileDialog(m_window);

    m_fileSelectionButton = new QPushButton("Выбрать файл", m_window);
    fileSelectionLayout->addWidget(m_fileSelectionButton);

    m_selectedFileLabel = new QLabel("");
    m_selectedFileLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    fileSelectionLayout->addWidget(m_selectedFileLabel);

    layout->addWidget(fileSelectionContainer);

    // Separator
    auto *separator = new QFrame;
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    layout->addWidget(separator);

    // Action widgets group
    auto *actionsContainer = new QWidget(m_window);
    auto *actionsLayout = new QHBoxLayout(actionsContainer);

    m_progressBar = new QProgressBar(m_window);
    m_progressBar->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    m_progressBar->setTextVisible(false);
    actionsLayout->addWidget(m_progressBar);

    m_verifyButton = new QPushButton("Проверить", m_window);
    actionsLayout->addWidget(m_verifyButton);

    m_writeButton = new QPushButton("Записать", m_window);
    actionsLayout->addWidget(m_writeButton);

    layout->addWidget(actionsContainer);

    //
    return container;
}


} // namespace app
