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
{
    m_window->setMinimumWidth(350);

    auto *container = new QWidget(m_window);
    auto *layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(createTopWorkspace());
    layout->addWidget(createBottomWorkspace());

    m_window->setCentralWidget(container);
}


void MainWindowUI::setViewMode(ViewMode mode)
{
    m_connectionWidget->setViewMode(mode);

    m_viewModeToggle->setText(mode == ViewMode::COMPACT ? "Расширенный режим" : "Простой режим");

    m_sectorSelectionWrapper->setCurrentIndex(mode == ViewMode::COMPACT ? 0 : 1);
}


void MainWindowUI::setConnectionState(app::ConnectionState state)
{
    m_connectionWidget->setConnectionState(state);
}


void MainWindowUI::setApplicationState(ApplicationState state)
{
    m_sectorSelectionWrapper->setEnabled(state != ApplicationState::DISCONNECTED);

    m_verifyButton->setEnabled(state != ApplicationState::DISCONNECTED);
    m_writeButton->setEnabled(state != ApplicationState::DISCONNECTED);
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

    // Sector selection
    m_sectorSelectionWrapper = new QStackedWidget(m_window);
    layout->addWidget(m_sectorSelectionWrapper);

    // Compact mode
    auto *sectorPresetsContainer = new QWidget(m_window);
    auto *sectorPresetsLayout = new QVBoxLayout(sectorPresetsContainer);

    m_sectorPresets = new SectorPresetsWidget(m_window);
    sectorPresetsLayout->addWidget(m_sectorPresets);
    sectorPresetsLayout->addStretch();

    m_sectorSelectionWrapper->addWidget(sectorPresetsContainer);

    m_sectorSelectionWrapper->addWidget(new QLabel("// TODO", m_window));

    //
    layout->addStretch();

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
