#include <iostream>
#include "lib/sensel.h"
#include "lib/sensel_device.h"
#include <array>
#include <atomic>

using namespace std;

struct Contact
{
    atomic<bool> state;
    atomic<float> force;
    atomic<float> x;
    atomic<float> y;
};

class Sensel
{
  public:
    Sensel()
    {
        // Get a list of avaialble Sensel devices
        // cout << "Devices available: " << list.num_devices << "\n";
        // cout << "name of device: " << list.devices << "\n";
        // cout << "Device 0: " << list.devices[0].idx << "\n";
        senselGetDeviceList(&list);

        if (list.num_devices == 0)
        {
            fprintf(stdout, "No Sensel device found.\n");
            return;
        }
        else
        {
            fprintf(stdout, "Sensel device detected.\n");
            senselDetected = true;
        }

        //Open a Sensel device by the id in the SenselDeviceList, handle initialized
        senselOpenDeviceByID(&handle, list.devices[0].idx);
        //Set the frame content to scan contact data
        senselSetFrameContent(handle, FRAME_CONTENT_CONTACTS_MASK);
        //Allocate a frame of data, must be done before reading frame data
        senselAllocateFrameData(handle, &frame);
        //Start scanning the Sensel device
        senselStartScanning(handle);
        
        for (int i = 0; i < mFingers.size(); i++)
            mFingers[i].state.store(false);
    }

    void getFingerPress(array<Contact, 20> &fingers)
    {
        unsigned long size = 0;

        if (mFingers.size() < fingers.size())
            size = mFingers.size();
        else
            size = fingers.size();

        for (int i = 0; i < size; i++)
        {
            fingers[i].state.store(mFingers[i].state.load());
            fingers[i].force.store(mFingers[i].force.load());
            fingers[i].x.store(mFingers[i].x.load());
            fingers[i].y.store(mFingers[i].y.load());
            
            //cout << "Finger[" << i << "]: " << fingers[i].state.load() << "\n";
        }
    }

    void check()
    {
        for (int i = 0; i < mFingers.size(); i++)
            mFingers[i].state.store(false);
        
        if (senselDetected)
        {
            unsigned int num_frames = 0;
            //Read all available data from the Sensel device
            senselReadSensor(handle);
            //Get number of frames available in the data read from the sensor
            senselGetNumAvailableFrames(handle, &num_frames);

            for (int f = 0; f < num_frames; f++)
            {
                //Read one frame of data
                senselGetFrame(handle, frame);

                // Get contact data
                if (frame->n_contacts > 0)
                {
                    for (int c = 0; c < frame->n_contacts; c++)
                    {
                        unsigned int state = frame->contacts[c].state;

                        if (state == CONTACT_START)
                        {
                            float force = frame->contacts[c].total_force;
                            float x = frame->contacts[c].x_pos;
                            float y = frame->contacts[c].y_pos;

                            if (c < mFingers.size())
                            {
                                mFingers[c].state.store(true);
                                mFingers[c].force.store(force);
                                mFingers[c].x.store(x);
                                mFingers[c].y.store(y);
                            }
                        }
                        else if(state == CONTACT_MOVE)
                        {
                            
                            float force = frame->contacts[c].total_force;
                            float x = frame->contacts[c].x_pos;
                            float y = frame->contacts[c].y_pos;

                            if (c < mFingers.size())
                            {
                                mFingers[c].state.store(true);
                                mFingers[c].force.store(force);
                                mFingers[c].x.store(x);
                                mFingers[c].y.store(y);
                            }
                        }
                        else if (state == CONTACT_END)
                        {
                            if (c < mFingers.size())
                            {
                                mFingers[c].state.store(false);
                                mFingers[c].force.store(0);
                                mFingers[c].x.store(0);
                                mFingers[c].y.store(0);
                            }
                        }
                    }
                }
            }
        }
        else
            fprintf(stdout, "No Sensel device found.\n");
    }

    array<Contact, 20> mFingers;
    bool senselDetected = false;

  private:
    SENSEL_HANDLE handle = NULL;
    //List of all available Sensel devices
    SenselDeviceList list;
    //SenselFrame data that will hold the contacts
    SenselFrameData *frame = NULL;
};
