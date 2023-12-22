#include <chrono>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>

/* Reference Count Pattern in C-oo Style
 * https://stackoverflow.com/questions/11788187/reference-counting-in-c-oo-style
 *
 * https://github.com/snaewe/loki-lib
 *
 * How to properly duplicate an ojbect given its shared_ptr?
 * https://stackoverflow.com/questions/22782288/how-to-properly-duplicate-an-object-given-its-shared-ptr
 * https://stackoverflow.com/questions/75742424/how-to-make-deep-a-copy-of-shared-ptr
 */

template <typename T>
struct WrapperPtr {
    typedef std::shared_ptr<T> Ptr;
    typedef std::shared_ptr<T const> ConstPtr;
};

struct SensorInput : WrapperPtr<SensorInput> {
    int time;

    SensorInput(int time_) : time(time_) {}
};

struct TrackFrame : WrapperPtr<TrackFrame> {
    void setSensorInput(const SensorInput::ConstPtr& ptr) { _sensor_input = ptr; }

    int getSensorInputTime() { return _sensor_input->time; }

  private:
    // Input Data Section
    SensorInput::ConstPtr _sensor_input = nullptr;
};

/*
 *
: Reset the _sensor_input and the reassignment won't affect the original one.
- 1. Copy the _sensor_input
- 2. Decouple the _sensor_input
- 3. reassign it to a new object

  a new object
    ^
    |               2
_sensor_input     --x-  > Heap memory
                        ^
                       / 1
the copy of sensor_input
 * */
struct FrameProvider {
    FrameProvider() { _track_frame = std::make_shared<TrackFrame>(); }

    void leftLeg(const SensorInput::ConstPtr& it) { setSensorInput(it); }

    void rightLeg() {
        // getReady
        TrackFrame::Ptr frame_copy = getStableFrameCopy();
        {
            std::lock_guard<std::mutex> lock(_mutex);
            //
            std::cout << "Processing ..." << std::endl;
        }

        // publish cleanup
        int time = frame_copy->getSensorInputTime();
        std::cout << "Current time: " << time << std::endl;
    }

    // private:
    void setSensorInput(const SensorInput::ConstPtr& ptr) { _track_frame->setSensorInput(ptr); }

    TrackFrame::Ptr getStableFrameCopy() {
        TrackFrame::Ptr current_frame;
        // Create a copy of _track_frame but still be put in a shared_ptr.
        current_frame = std::make_shared<TrackFrame>(*_track_frame);
        return current_frame;
    }

  private:
    TrackFrame::Ptr _track_frame = nullptr;
    std::mutex _mutex;
};

class InputProvider {
  public:
    void operator()(FrameProvider& p) {
        std::cout << "InputHandler thread" << std::endl;
        SensorInput::Ptr i = std::make_shared<SensorInput>(1);
        p.leftLeg(i);
    }
};

class DataProcessing {
  public:
    void process(FrameProvider& p) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        p.rightLeg();
    }
};

int main() {
    FrameProvider p;
    InputProvider ih;
    // https://en.cppreference.com/w/cpp/thread/thread/thread
    std::thread input_handler_thread(ih, std::ref(p));

    DataProcessing dp;
    std::thread data_processing_thread(&DataProcessing::process, &dp, std::ref(p));

    input_handler_thread.join();
    data_processing_thread.join();

    /*
        {  // bad case
            // 1. getReady
            TrackFrame::Ptr frame_copy = p.getStableFrameCopy();
            //
            std::cout << "Processing ..." << std::endl;
            // 2. During the processing, we change TrackFrame's member: _sensor_input
            SensorInput::Ptr i = std::make_shared<SensorInput>(2);
            p.setSensorInput(i);

            // 2. During the processing, we also re-get the frame_copy for enough time
            frame_copy = p.getStableFrameCopy();
            // 3. publish cleanup
            int time = frame_copy->getSensorInputTime();
            std::cout << "Current time: " << time << std::endl;

            // Check the latest value:
            { std::cout << "Latest current time: " << p.getStableFrameCopy()->getSensorInputTime() << std::endl; }
        }
    */
}
