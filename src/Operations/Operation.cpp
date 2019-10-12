/*
 * This is an open source non-commercial project. Dear PVS-Studio, please check it.
 * PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
 */
#include "Operation.h"

#include <QMessageBox>
#include <QPushButton>

namespace app
{
Operation::Operation(Programmer &programmer, const SectorPresetsModel &model, const QString &name)
    : m_name{name}
    , m_programmer{programmer}
    , m_sectorPresetsModel{model}
    , m_selectedPresets{model.getSelectedPresets()}
{
}


const QString &Operation::getName() const
{
    return m_name;
}


Programmer &Operation::getProgrammer() const
{
    return m_programmer;
}


void Operation::requestCancellation()
{
    m_shouldStop = true;
}


void Operation::finishCancellation(bool result)
{
    m_cancellationMutex.lock();
    m_cancellationResult = result;
    m_cancellationMutex.unlock();
    m_cancellationGuard.notify_one();
}


const SectorPresetsModel &Operation::getSectorPresetsModel() const
{
    return m_sectorPresetsModel;
}


const std::vector<std::pair<SectorPresetsModel::Preset, size_t>> &Operation::getSelectedPresets() const
{
    return m_selectedPresets;
}


void Operation::checkCancelled()
{
    if (!m_shouldStop)
    {
        return;
    }

    m_cancellationResult.reset();
    emit showCancellationDialog();

    std::unique_lock<std::mutex> lock{m_cancellationMutex};
    m_cancellationGuard.wait(lock, [this] { return m_cancellationResult.has_value(); });

    m_shouldStop = *m_cancellationResult;

    if (*m_cancellationResult)
    {
        throw CancelledException{};
    }
}

} // namespace app
