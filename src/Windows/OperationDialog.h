#ifndef PROGRAMMER_SRC_WINDOWS_OPERATIONDIALOG_H_
#define PROGRAMMER_SRC_WINDOWS_OPERATIONDIALOG_H_

#include <memory>

#include <QDialog>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>

#include "Operations/Operation.h"

namespace app
{
class OperationDialog : public QDialog
{
    Q_OBJECT

public:
    enum class State
    {
        CONFIRMATION,
        PROGRESS,
        CONCLUSION
    };

    explicit OperationDialog(std::unique_ptr<Operation> operation, QWidget *parent = nullptr);

    OperationDialog(const OperationDialog &) = delete;
    OperationDialog &operator=(const OperationDialog &) = delete;
    OperationDialog(OperationDialog &&) noexcept = delete;
    OperationDialog &operator=(OperationDialog &&) noexcept = delete;

private:
    void setState(State state);

    void createUI();
    void createConfirmationTab();
    void createProgressTab();

    void connectSignals();

    void onProgress(int progress, const QString &message);
    void onComplete(bool success);

private:
    State m_state;
    std::unique_ptr<Operation> m_operation;

    QWidget *m_confirmationTab = nullptr;
    QWidget *m_progressTab = nullptr;

    QPushButton *m_confirmationAcceptButton = nullptr;
    QPushButton *m_confirmationRejectButton = nullptr;

    QLabel *m_progressLabel = nullptr;
    QProgressBar *m_progressBar = nullptr;

    QPushButton *m_conclusionButton = nullptr;
};

} // namespace app

#endif // PROGRAMMER_SRC_WINDOWS_OPERATIONDIALOG_H_
