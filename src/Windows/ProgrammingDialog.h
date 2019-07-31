#ifndef PROGRAMMER_SRC_WINDOWS_PROGRAMMINGDIALOG_H_
#define PROGRAMMER_SRC_WINDOWS_PROGRAMMINGDIALOG_H_

#include <QDialog>
#include <QPushButton>

namespace app
{
class ProgrammingDialog : public QDialog
{
    Q_OBJECT

public:
    enum class State
    {
        CONFIRMATION,
        PROGRESS,
        CONCLUSION
    };

    explicit ProgrammingDialog(const QString &fileName, QWidget *parent = nullptr);

    ProgrammingDialog(const ProgrammingDialog &) = delete;
    ProgrammingDialog &operator=(const ProgrammingDialog &) = delete;
    ProgrammingDialog(ProgrammingDialog &&) noexcept = delete;
    ProgrammingDialog &operator=(ProgrammingDialog &&) noexcept = delete;

private:
    void setState(State state);

    void createUI();
    void connectSignals();

    State m_state;
    QString m_fileName;

    QWidget *m_confirmationContainer = nullptr;
    QWidget *m_progressContainer = nullptr;
    QWidget *m_conclusionContainer = nullptr;

    QPushButton *m_confirmationAcceptButton = nullptr;
    QPushButton *m_confirmationRejectButton = nullptr;
};

} // namespace app

#endif // PROGRAMMER_SRC_WINDOWS_PROGRAMMINGDIALOG_H_
