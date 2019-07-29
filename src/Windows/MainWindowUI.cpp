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

namespace app
{
MainWindowUI::MainWindowUI(QMainWindow *window)
    : m_window{window}
    , m_isExtended{false}
{
    m_window->setMinimumWidth(350);

    auto *container = new QWidget(m_window);
    auto *layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(createTopWorkspace());
    layout->addWidget(createBottomWorkspace());

    m_window->setCentralWidget(container);

    setExtended(m_isExtended);
}


void MainWindowUI::toggleExtended()
{
    setExtended(!m_isExtended);
}


void MainWindowUI::setExtended(bool extended)
{
    m_connectionWidget->setViewMode(extended ? ConnectionWidget::ViewMode::EXTENDED
                                             : ConnectionWidget::ViewMode::COMPACT);

    m_viewModeToggle->setText(extended ? "Простой режим" : "Расширенный режим");

    m_isExtended = extended;
}


bool MainWindowUI::isExtended() const
{
    return m_isExtended;
}


ConnectionWidget *MainWindowUI::getConnectionWidget() const
{
    return m_connectionWidget;
}


LinkButton *MainWindowUI::getViewModeToggle() const
{
    return m_viewModeToggle;
}


QPushButton *MainWindowUI::getVerifyButton() const
{
    return m_verifyButton;
}


QPushButton *MainWindowUI::getWriteButton() const
{
    return m_writeButton;
}


QWidget *MainWindowUI::createTopWorkspace()
{
    auto *container = new QWidget(m_window);
    auto *layout = new QVBoxLayout(container);

    container->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    // Serial port selector
    m_connectionWidget = new ConnectionWidget(m_window);
    layout->addWidget(m_connectionWidget);

    layout->addStretch();

    //
    return container;
}


QWidget *MainWindowUI::createBottomWorkspace()
{
    auto *container = new QWidget(m_window);
    auto *layout = new QVBoxLayout(container);

    // View mode toggle
    auto *viewModeContainer = new QWidget(m_window);
    auto viewModeLayout = new QHBoxLayout(viewModeContainer);

    m_viewModeToggle = new LinkButton(m_window);
    viewModeLayout->addWidget(m_viewModeToggle);
    viewModeLayout->addStretch();

    layout->addWidget(viewModeContainer);

    // Separator
    auto *separator = new QFrame;
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    layout->addWidget(separator);

    // Action widgets group
    auto *actionsContainer = new QWidget(m_window);
    auto *actionsLayout = new QHBoxLayout(actionsContainer);

    m_verifyButton = new QPushButton("Проверить", m_window);
    actionsLayout->addWidget(m_verifyButton);

    m_writeButton = new QPushButton("Записать", m_window);
    actionsLayout->addWidget(m_writeButton);

    layout->addWidget(actionsContainer);

    //
    return container;
}

} // namespace app
