#include <iostream>
#include <memory>

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
};

struct SensorInput: WrapperPtr<SensorInput> {
    int time;
    SensorInput(int time_): time(time_) {}
    SensorInput(const SensorInput& other) {
        this->time = other.time;
        std::cout << "SensorInput: Copy consstructor" << std::endl;
    }
};

struct TrackFrame: WrapperPtr<TrackFrame> {
    TrackFrame() {
        _sensor_input = std::make_shared<SensorInput>(0);
    }

    TrackFrame(const TrackFrame& other) {
        std::cout << "TrackFrame: Copy consstructor" << std::endl;
        this->_sensor_input = std::make_shared<SensorInput>(*other._sensor_input);
    }

    void setSensorInput(int time) {
        _sensor_input-> time = time;
    }

    int getSensorInputTime() {
        return _sensor_input-> time;
    }

    private:
    // Input Data Section
    SensorInput::Ptr _sensor_input;
};

/*
 *

|TrackFrame    |
---------------|
|_sensor_input |
---------------|

_sensor_input     ---  > Heap memory
                        ^
                       /                 
the copy of sensor_input 
 * */
struct FrameProvider {
    FrameProvider() { 
        _track_frame = std::make_shared<TrackFrame>();
    }


    void leftLeg(int it) {
        setSensorInput(it);
    }

    void rightLeg()  {
        // getReady
        TrackFrame::Ptr frame_copy = getStableFrameCopy();
        // 
        std::cout << "Processing ..." << std::endl;

        // publish cleanup
        int time = frame_copy->getSensorInputTime();
        std::cout << "Current time: " << time << std::endl;
    }

//private:
    void setSensorInput(int time) {
        _track_frame->setSensorInput(time);
    }

    TrackFrame::Ptr getStableFrameCopy() {
        TrackFrame::Ptr current_frame;
        /*
            std::make_shared<TrackFrame, TrackFrame&> () at /usr/include/c++/11/bits/shared_ptr.h:875
      make_shared(_Args&&... __args)
     *  @throw  std::bad_alloc, or an exception thrown from the
     *          constructor of @a _Tp.
        template<typename _Tp, typename... _Args>
          inline shared_ptr<_Tp>
          make_shared(_Args&&... __args)
          {
            typedef typename std::remove_cv<_Tp>::type _Tp_nc;
            return std::allocate_shared<_Tp>(std::allocator<_Tp_nc>(),
          			       std::forward<_Args>(__args)...);
          }
         * */
        // Create a copy of _track_frame but still be put in a shared_ptr.
        current_frame = std::make_shared<TrackFrame>(*_track_frame);
        return current_frame;
    }

private:
    TrackFrame::Ptr    _track_frame;
};

// Solution: using deep copy
int main() {
    FrameProvider p;
    p.leftLeg(1);
    p.rightLeg();

    { // bad case
        p.leftLeg(1);

        // getReady
        TrackFrame::Ptr frame_copy = p.getStableFrameCopy();
        // 
        std::cout << "Processing ..." << std::endl;
        p.setSensorInput(2);

        // publish cleanup
        int time = frame_copy->getSensorInputTime();
        std::cout << "Current time: " << time << std::endl;
        {
        std::cout << "Again, Current time: " << p.getStableFrameCopy()->getSensorInputTime()
            << std::endl;
    }

    }

}

