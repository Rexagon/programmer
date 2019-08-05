/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include "OperationDialog.h"

#include <thread>

#include <QHBoxLayout>
#include <QVBoxLayout>

namespace
{
constexpr auto DEFAULT_STATE = app::OperationDialog::State::CONFIRMATION;
}


namespace app
{
OperationDialog::OperationDialog(std::unique_ptr<Operation> operation, QWidget *parent)
    : QDialog{parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint}
    , m_state{DEFAULT_STATE}
    , m_operation{std::move(operation)}
{
    createUI();
    connectSignals();

    setState(m_state);
}


void OperationDialog::setState(const State state)
{
    m_state = state;

    m_confirmationTab->setVisible(state == State::CONFIRMATION);
    m_progressTab->setVisible(state == State::PROGRESS || state == State::CONCLUSION);

    m_conclusionButton->setEnabled(state == State::CONCLUSION);

    QString title;
    switch (state)
    {
        case State::CONFIRMATION:
            title = "Подтвертите действие";
            break;

        case State::PROGRESS:
            title = "Подождите";
            break;

        case State::CONCLUSION:
            title = "Готово";
            break;

        default:
            break;
    }

    setWindowTitle(QString("%1 | %2").arg(m_operation->getName(), title));
}


void OperationDialog::createUI()
{
    setMinimumSize(350, 120);

    createConfirmationTab();
    createProgressTab();

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(m_confirmationTab);
    layout->addWidget(m_progressTab);
}


void OperationDialog::createConfirmationTab()
{
    m_confirmationTab = new QWidget(this);
    auto *confirmationLayout = new QVBoxLayout(m_confirmationTab);

    confirmationLayout->setContentsMargins(0, 0, 0, 0);
    confirmationLayout->setSpacing(0);

    // Content begin
    auto *contentContainer = new QWidget(this);
    auto *contentLayout = new QVBoxLayout(contentContainer);

    // Label
    auto *confirmationLabel = new QLabel(this);
    confirmationLabel->setText(m_operation->getDescription());
    contentLayout->addWidget(confirmationLabel);

    confirmationLayout->addWidget(contentContainer);
    // Content end

    confirmationLayout->addStretch();

    // Actions separator
    auto *actionsSeparator = new QFrame;
    actionsSeparator->setFrameShape(QFrame::HLine);
    actionsSeparator->setFrameShadow(QFrame::Sunken);
    confirmationLayout->addWidget(actionsSeparator);

    // Actions
    auto *confirmationActionsContainer = new QWidget(this);
    auto *confirmationActionsLayout = new QHBoxLayout(confirmationActionsContainer);

    m_confirmationRejectButton = new QPushButton("Отменить", this);
    confirmationActionsLayout->addWidget(m_confirmationRejectButton);

    m_confirmationAcceptButton = new QPushButton("Принять", this);
    confirmationActionsLayout->addWidget(m_confirmationAcceptButton);

    confirmationLayout->addWidget(confirmationActionsContainer);
}


void OperationDialog::createProgressTab()
{
    m_progressTab = new QWidget(this);
    auto *progressLayout = new QVBoxLayout(m_progressTab);

    progressLayout->setContentsMargins(0, 0, 0, 0);
    progressLayout->setSpacing(0);

    // Content begin
    auto *contentContainer = new QWidget(this);
    auto *contentLayout = new QVBoxLayout(contentContainer);

    // Label
    m_progressLabel = new QLabel(this);
    contentLayout->addWidget(m_progressLabel);

    // Progress bar
    m_progressBar = new QProgressBar(this);
    m_progressBar->setTextVisible(false);
    m_progressBar->setRange(0, 100);
    contentLayout->addWidget(m_progressBar);

    progressLayout->addWidget(contentContainer);
    // Content end

    progressLayout->addStretch();

    // Actions separator
    auto *actionsSeparator = new QFrame;
    actionsSeparator->setFrameShape(QFrame::HLine);
    actionsSeparator->setFrameShadow(QFrame::Sunken);
    progressLayout->addWidget(actionsSeparator);

    // Actions
    auto *actionsContainer = new QWidget(this);
    auto *actionsLayout = new QHBoxLayout(actionsContainer);

    actionsLayout->addStretch();

    m_conclusionButton = new QPushButton("Готово", this);
    actionsLayout->addWidget(m_conclusionButton);

    progressLayout->addWidget(actionsContainer);
}


void OperationDialog::connectSignals()
{
    connect(m_confirmationRejectButton, &QPushButton::clicked, [this] { close(); });
    connect(m_confirmationAcceptButton, &QPushButton::clicked, [this] {
        setState(State::PROGRESS);

        std::thread{[this] { m_operation->run(); }}.detach();
    });

    connect(m_operation.get(), &Operation::notifyProgress, this, &OperationDialog::onProgress);
    connect(m_operation.get(), &Operation::notifyComplete, this, &OperationDialog::onComplete);

    connect(m_conclusionButton, &QPushButton::clicked, [this] { close(); });
}


void OperationDialog::onProgress(int progress, const QString &message)
{
    m_progressLabel->setText(message);
    m_progressBar->setValue(progress);
}


void OperationDialog::onComplete(bool /*success*/)
{
    setState(State::CONCLUSION);
}

} // namespace app
