#ifndef GSF_H
#define GSF_H

bool mbox_head_frame_present(mbox_t *mbox,int flowId);
bool HeadFrameActive(int flowId);
void UpdateHeadFrame();
int FindFrameNumber(int flowId, int packetSize);
void initGSF(int Framesize, int WindowSize);
int calcGSFPriority(int FrameNum, int Node);
#endif
