/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */

#include "ProgrammingDialog.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

namespace
{
constexpr auto DEFAULT_STATE = app::ProgrammingDialog::State::CONFIRMATION;
}


namespace app
{
ProgrammingDialog::ProgrammingDialog(const QString &fileName, QWidget *parent)
    : QDialog{parent, Qt::WindowSystemMenuHint | Qt::WindowTitleHint}
    , m_state{DEFAULT_STATE}
    , m_fileName{fileName}
{
    createUI();
    connectSignals();

    setState(m_state);
}


void ProgrammingDialog::setState(const State state)
{
    m_state = state;

    m_confirmationContainer->setVisible(state == State::CONFIRMATION);
    m_progressContainer->setVisible(state == State::PROGRESS);
    m_conclusionContainer->setVisible(state == State::CONCLUSION);

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
    setWindowTitle(title);
}


void ProgrammingDialog::createUI()
{
    auto *layout = new QVBoxLayout{this};

    layout->setContentsMargins(0, 0, 0, 0);

    // Confirmation container
    m_confirmationContainer = new QWidget{this};
    auto *confirmationLayout = new QVBoxLayout{m_confirmationContainer};

    confirmationLayout->setContentsMargins(0, 0, 0, 0);

    //      Labels
    auto *confirmationLabelsContainer = new QWidget{this};
    auto *confirmationLabelsLayout = new QVBoxLayout{confirmationLabelsContainer};

    auto *confirmationFileLabel = new QLabel{"Запись файла прошивки:", this};
    confirmationLabelsLayout->addWidget(confirmationFileLabel);

    auto *fileNameLabel = new QLabel{"    " + m_fileName, this};
    confirmationLabelsLayout->addWidget(fileNameLabel);

    auto *confirmationSectorsLabel = new QLabel{"В секторы:", this};
    confirmationLabelsLayout->addWidget(confirmationSectorsLabel);

    auto *sectorsLabel = new QLabel{"   SA0, SA1, SA2, SA3", this};
    confirmationLabelsLayout->addWidget(sectorsLabel);

    confirmationLayout->addWidget(confirmationLabelsContainer);

    //      Actions separator
    auto *actionsSeparator = new QFrame;
    actionsSeparator->setFrameShape(QFrame::HLine);
    actionsSeparator->setFrameShadow(QFrame::Sunken);
    confirmationLayout->addWidget(actionsSeparator);

    //      Actions
    auto *confirmationActionsContainer = new QWidget{this};
    auto *confirmationActionsLayout = new QHBoxLayout{confirmationActionsContainer};

    m_confirmationRejectButton = new QPushButton{"Отменить", this};
    confirmationActionsLayout->addWidget(m_confirmationRejectButton);

    m_confirmationAcceptButton = new QPushButton{"Принять", this};
    confirmationActionsLayout->addWidget(m_confirmationAcceptButton);

    confirmationLayout->addWidget(confirmationActionsContainer);

    //
    layout->addWidget(m_confirmationContainer);

    // Progress container
    m_progressContainer = new QWidget{this};
    /*auto *progressLayout =*/new QVBoxLayout{m_progressContainer};

    layout->addWidget(m_progressContainer);

    // Conclusion container
    m_conclusionContainer = new QWidget{this};
    /*auto *conclusionLayout = */ new QVBoxLayout{m_conclusionContainer};

    layout->addWidget(m_conclusionContainer);
}


void ProgrammingDialog::connectSignals()
{
    connect(m_confirmationRejectButton, &QPushButton::clicked, [this] { close(); });
}

} // namespace app
