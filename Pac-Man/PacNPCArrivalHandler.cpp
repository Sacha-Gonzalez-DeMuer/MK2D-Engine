#include "PacNPCArrivalHandler.h"
#include "PacNPC.h"
#include "PacNPCState.h"

void dae::PacNPCArrivalHandler::operator()(int, std::shared_ptr<PacGrid>)
{
     m_pNPC->GetState()->OnArrive(*m_pNPC);
}
