/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include "MainWindowUI.h"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

namespace
{
constexpr auto WINDOW_WIDTH_MIN = 275;
}

namespace app
{
MainWindowUI::MainWindowUI(QMainWindow *window)
    : m_window{window}
{
    m_window->setWindowTitle("Программатор");
    m_window->setMinimumWidth(WINDOW_WIDTH_MIN);

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

    m_viewModeToggle->setText(toString(!mode));

    m_sectorsSelectionWrapper->setCurrentIndex(mode == ViewMode::COMPACT ? 0 : 1);

    m_dumpButton->setVisible(mode == ViewMode::EXTENDED);
    m_clearButton->setVisible(mode == ViewMode::EXTENDED);
}


void MainWindowUI::setConnectionState(app::ConnectionState state)
{
    m_connectionWidget->setConnectionState(state);
}


void MainWindowUI::setApplicationState(ApplicationState state)
{
    m_sectorsSelectionWrapper->setEnabled(state != ApplicationState::DISCONNECTED);

    m_dumpButton->setEnabled(state != ApplicationState::DISCONNECTED);
    m_clearButton->setEnabled(state != ApplicationState::DISCONNECTED);
    m_verifyButton->setEnabled(state != ApplicationState::DISCONNECTED);
    m_writeButton->setEnabled(state != ApplicationState::DISCONNECTED);
}


void MainWindowUI::setSectorsTableModel(SectorTableModel *model)
{
    m_sectorPresets->setModel(model);
    m_sectorTable->setModel(model);
}


ConnectionWidget *MainWindowUI::getConnectionWidget() const
{
    return m_connectionWidget;
}


LinkButton *MainWindowUI::getViewModeToggle() const
{
    return m_viewModeToggle;
}


QPushButton *MainWindowUI::getDumpButton() const
{
    return m_dumpButton;
}


QPushButton *MainWindowUI::getClearButton() const
{
    return m_clearButton;
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

    layout->setContentsMargins(0, 0, 0, 0);

    // Serial port selector
    m_connectionWidget = new ConnectionWidget(m_window);
    layout->addWidget(m_connectionWidget);

    // Sector selection
    m_sectorsSelectionWrapper = new QStackedWidget(m_window);
    m_sectorsSelectionWrapper->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    layout->addWidget(m_sectorsSelectionWrapper);

    // Compact mode
    auto *sectorPresetsContainer = new QWidget(m_window);
    auto *sectorPresetsLayout = new QVBoxLayout(sectorPresetsContainer);

    m_sectorPresets = new SectorPresetsWidget(m_window);
    sectorPresetsLayout->addWidget(m_sectorPresets);
    sectorPresetsLayout->addStretch();

    m_sectorsSelectionWrapper->addWidget(sectorPresetsContainer);

    // Extended mode
    auto *sectorsTableContainer = new QWidget(m_window);
    auto *sectorsTableLayout = new QVBoxLayout(sectorsTableContainer);

    m_sectorTable = new SectorTableWidget(m_window);
    sectorsTableLayout->addWidget(m_sectorTable);

    m_sectorsSelectionWrapper->addWidget(sectorsTableContainer);

    //
    return container;
}


QWidget *MainWindowUI::createBottomWorkspace()
{
    auto *container = new QWidget(m_window);
    auto *layout = new QVBoxLayout(container);

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

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
    auto *actionsLayout = new QGridLayout(actionsContainer);

    m_dumpButton = new QPushButton("Считать", m_window);
    actionsLayout->addWidget(m_dumpButton, 0, 0);

    m_clearButton = new QPushButton("Очистить", m_window);
    actionsLayout->addWidget(m_clearButton, 0, 1);

    m_verifyButton = new QPushButton("Проверить", m_window);
    actionsLayout->addWidget(m_verifyButton, 1, 0);

    m_writeButton = new QPushButton("Записать", m_window);
    actionsLayout->addWidget(m_writeButton, 1, 1);

    layout->addWidget(actionsContainer);

    //
    return container;
}

} // namespace app
