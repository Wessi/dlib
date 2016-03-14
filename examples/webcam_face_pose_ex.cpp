// The contents of this file are in the public domain. See LICENSE_FOR_EXAMPLE_PROGRAMS.txt
/*

    This example program shows how to find frontal human faces in an image and
    estimate their pose.  The pose takes the form of 68 landmarks.  These are
    points on the face such as the corners of the mouth, along the eyebrows, on
    the eyes, and so forth.  
    

    This example is essentially just a version of the face_landmark_detection_ex.cpp
    example modified to use OpenCV's VideoCapture object to read from a camera instead 
    of files.


    Finally, note that the face detector is fastest when compiled with at least
    SSE2 instructions enabled.  So if you are using a PC with an Intel or AMD
    chip then you should enable at least SSE2 instructions.  If you are using
    cmake to compile this program you can enable them by using one of the
    following commands when you create the build project:
        cmake path_to_dlib_root/examples -DUSE_SSE2_INSTRUCTIONS=ON
        cmake path_to_dlib_root/examples -DUSE_SSE4_INSTRUCTIONS=ON
        cmake path_to_dlib_root/examples -DUSE_AVX_INSTRUCTIONS=ON
    This will set the appropriate compiler options for GCC, clang, Visual
    Studio, or the Intel compiler.  If you are using another compiler then you
    need to consult your compiler's manual to determine how to enable these
    instructions.  Note that AVX is the fastest but requires a CPU from at least
    2011.  SSE4 is the next fastest and is supported by most current machines.  
*/

#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>

using namespace dlib;
using namespace std;

int main()
{
    double lmPoints[68][2];//  = {{3,4},{5,6},{7,8},{0,1}};
    bool flag = false;
    int cnt = 0;
    int s1, s2, s3, s4, s5, s6;
    try
    {
        cv::VideoCapture cap(0);
        image_window win;
        // for (int i = 0; i < 4; ++i)
        // {
        //     cout<<"the pair:{"<<lmPoints[i][0]<<", "<<lmPoints[i][1]<<"}\n";
        // }
        // Load face detection and pose estimation models.
        frontal_face_detector detector = get_frontal_face_detector();
        shape_predictor pose_model;
        deserialize("shape_predictor_68_face_landmarks.dat") >> pose_model;

        // Grab and process frames until the main window is closed by the user.
        while(!win.is_closed())
        {
            // Grab a frame
            cv::Mat temp;
            cap >> temp;
            // Turn OpenCV's Mat into something dlib can deal with.  Note that this just
            // wraps the Mat object, it doesn't copy anything.  So cimg is only valid as
            // long as temp is valid.  Also don't do anything to temp that would cause it
            // to reallocate the memory which stores the image as that will make cimg
            // contain dangling pointers.  This basically means you shouldn't modify temp
            // while using cimg.
            cv_image<bgr_pixel> cimg(temp);

            // Detect faces 
            std::vector<rectangle> faces = detector(cimg);
            // Find the pose of each face.
            std::vector<full_object_detection> shapes;
            for (unsigned long i = 0; i < faces.size(); ++i)
                shapes.push_back(pose_model(cimg, faces[i]));

            // Display it all on the screen
            win.clear_overlay();
            win.set_image(cimg);
            win.add_overlay(render_face_detections(shapes));

///////////////////////////////////
            for (unsigned long i = 0; i < shapes.size(); ++i)
            {
                
                const full_object_detection& d = shapes[i];
                for (unsigned long i = 0; i < 68; ++i)
                {
                    lmPoints[i][0] = (double)d.part(i).x();
                    lmPoints[i][1] = (double)d.part(i).y();
                    //std::cout<<"i: "<<i<<" X: "<<lmPoints[i][0]<<"="<<d.part(i).x()<<" Y: "<<lmPoints[i][1]<<"="<<d.part(i).y()<<std::endl; 
                    if (!flag)
                    {
                        flag = true;                        
                    }
                }
                //std::cout<<std::endl; 
            }
            
            if(flag){
                /*
                find the area of eye: https://www.mathsisfun.com/geometry/area-irregular-polygons.html
                for each value that are obtained by different eye actions
                publish some action on the screen 

                eg if(le<re --> write left eye close... etc)
                */
                double diffLeftEye = lmPoints[47][1] - lmPoints[43][1];
                double diffRightEye = lmPoints[40][1] - lmPoints[38][1];  
                double s2 = lmPoints[38][0] - lmPoints[37][0];  
                double s5 = lmPoints[41][0] - lmPoints[40][0];  

                cout<<"Side2: test my code @icog: "<<s2<<endl;
                cout<<"Side5: test my code @icog: "<<s5<<endl;
           
                cout<<"Left eye: test my code @icog: "<<diffLeftEye<<endl;
                cout<<"Right eye: test my code @icog: "<<diffRightEye<<endl;
                
                for (int i = 0; i < sizeof(lmPoints) / sizeof(*lmPoints); ++i)
                {

                    //cout<<"LMP-"<<i<<":{"<<lmPoints[i][0]<<", "<<lmPoints[i][1]<<"}\n";
                }
                flag = false;
            }
////////////////////////////

        }
        
    }
    catch(serialization_error& e)
    {
        cout << "You need dlib's default face landmarking model file to run this example." << endl;
        cout << "You can get it from the following URL: " << endl;
        cout << "   http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2" << endl;
        cout << endl << e.what() << endl;
    }
    catch(exception& e)
    {
        cout << e.what() << endl;
    }
    /*
    */
}

