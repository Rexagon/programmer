/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include "MainWindowUI.h"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

#include "../General.h"

namespace
{
constexpr auto WINDOW_WIDTH_MIN = 275;
}

namespace app
{
MainWindowUI::MainWindowUI(SectorPresetsModel &model, QMainWindow *window)
    : m_window{window}
{
    m_window->setWindowTitle("Программатор");
    m_window->setFixedWidth(WINDOW_WIDTH_MIN);

    auto *container = new QWidget(m_window);
    auto *layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(createTopWorkspace(model));
    layout->addWidget(createBottomWorkspace());

    m_window->setCentralWidget(container);
}


void MainWindowUI::setConnectionState(app::ConnectionState state)
{
    m_connectionWidget->setConnectionState(state);
}


void MainWindowUI::setApplicationState(ApplicationState state)
{
    m_sectorPresetsWidget->setEnabled(state != ApplicationState::DISCONNECTED);

    m_dumpButton->setEnabled(state != ApplicationState::DISCONNECTED);
    m_verifyButton->setEnabled(state != ApplicationState::DISCONNECTED);
    m_writeButton->setEnabled(state != ApplicationState::DISCONNECTED);
}


ConnectionWidget *MainWindowUI::getConnectionWidget() const
{
    return m_connectionWidget;
}


QPushButton *MainWindowUI::getDumpButton() const
{
    return m_dumpButton;
}


QPushButton *MainWindowUI::getVerifyButton() const
{
    return m_verifyButton;
}


QPushButton *MainWindowUI::getWriteButton() const
{
    return m_writeButton;
}


QWidget *MainWindowUI::createTopWorkspace(SectorPresetsModel &model)
{
    auto *container = new QWidget(m_window);
    auto *layout = new QVBoxLayout(container);

    layout->setContentsMargins(0, 0, 0, 0);

    // Serial port selector
    m_connectionWidget = new ConnectionWidget(m_window);
    layout->addWidget(m_connectionWidget);

    // Compact mode
    auto *sectorPresetsContainer = new QWidget(m_window);
    auto *sectorPresetsLayout = new QVBoxLayout(sectorPresetsContainer);

    m_sectorPresetsWidget = new SectorPresetsWidget(model, m_window);
    sectorPresetsLayout->addWidget(m_sectorPresetsWidget);

    layout->addWidget(sectorPresetsContainer);

    //
    layout->addStretch();

    //
    auto *versionLabelContainer = new QWidget(m_window);
    auto *versionLabelLayout = new QHBoxLayout(versionLabelContainer);

    versionLabelLayout->addStretch();

    auto *versionLabel = new QLabel(QString("Версия: %1").arg(APP_VERSION), m_window);
    versionLabelLayout->addWidget(versionLabel);

    layout->addWidget(versionLabelContainer);

    //
    return container;
}


QWidget *MainWindowUI::createBottomWorkspace()
{
    auto *container = new QWidget(m_window);
    auto *layout = new QVBoxLayout(container);

    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    // Separator
    auto *separator = new QFrame;
    separator->setFrameShape(QFrame::HLine);
    separator->setFrameShadow(QFrame::Sunken);
    layout->addWidget(separator);

    // Action widgets group
    auto *actionsContainer = new QWidget(m_window);
    auto *actionsLayout = new QVBoxLayout(actionsContainer);

    m_dumpButton = new QPushButton("Считать", m_window);
    m_dumpButton->setToolTip("Считать текущую прошивку в файлы");
    actionsLayout->addWidget(m_dumpButton);

    m_verifyButton = new QPushButton("Проверить", m_window);
    m_verifyButton->setToolTip("Сравнить прошивку в секторах с выбранным файлом");
    actionsLayout->addWidget(m_verifyButton);

    m_writeButton = new QPushButton("Записать", m_window);
    m_writeButton->setToolTip("Записать файл прошивки в выбранные сектора");
    actionsLayout->addWidget(m_writeButton);

    layout->addWidget(actionsContainer);

    //
    return container;
}

} // namespace app
