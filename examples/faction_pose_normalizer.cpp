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
#include <math.h>
#include <dlib/opencv.h>
#include <opencv2/highgui/highgui.hpp>
#include <dlib/image_processing/frontal_face_detector.h>
#include <dlib/image_processing/render_face_detections.h>
#include <dlib/image_processing.h>
#include <dlib/gui_widgets.h>
#include <unistd.h>

using namespace dlib;
using namespace std;
double areaOfShape(double points[68][2], long rows);
int main()
{

    double lmPoints[68][2];
    double first100ds[500] = {};
    double intermediateaR[500] = {};
    double max = 0, cur = 0;
    double verticesLE[68][2] = {{}};
    double verticesRE[68][2] = {{}};
    double lipsSmileL[68][2] = {{}};
    double lipsSmileR[68][2] = {{}};
    double verticesIO[68][2] = {{}};
    double lipsSmileLi[68][2] = {{}};
    double lipsSmileRi[68][2] = {{}};
    double coordinates[68][2] = {{3,4}, {5,11}, {12,8}, {9,5},{5,6}};
    bool faceDetected = false;
    bool captured = false;
    int cnt = 0; 
    int icnt = 0;
    int s1, s2, s3, s4, s5, s6;
    double first100dsAvg = 0;
    double bv = 0;
    double roundOff_bv = 0.000;
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
            // usleep(1000000/2);//sleep for 1 second
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

            ///////////////////////////////////added
            // if (shapes.size()>0)
            // {                
               // cout<<"\nNo. of faces faceDetected:"<<shapes.size()<<endl;
            // }
            if(shapes.size()>0){
                for (unsigned long i = 0; i < shapes.size(); ++i)
                {                    
                    const full_object_detection& d = shapes[i];
                    for (unsigned long i = 0; i < 68; ++i)
                    {
                        lmPoints[i][0] = (double)d.part(i).x();
                        lmPoints[i][1] = (double)d.part(i).y();
                        //std::cout<<"i: "<<i<<" X: "<<lmPoints[i][0]<<"="<<d.part(i).x()<<" Y: "<<lmPoints[i][1]<<"="<<d.part(i).y()<<std::endl; 
                        // if (!faceDetected)
                        // {
                        //     faceDetected = true;            
                        // }
                    }
                    //std::cout<<std::endl; 
                }
                   /*
                    find the area of polygon
                    for each value that are obtained by different eye or mouse actions
                    publish some action on the screen 
                    eg if(le<re --> write left eye close... etc)
                    */
                    double diffLeftEye = lmPoints[47][1] - lmPoints[43][1];
                    double diffRightEye = lmPoints[40][1] - lmPoints[38][1];  
                    double s2 = lmPoints[38][0] - lmPoints[37][0];  
                    double s5 = lmPoints[41][0] - lmPoints[40][0];  

                    // cout<<"Side2: test my code @icog: "<<s2<<endl;
                    // cout<<"Side5: test my code @icog: "<<s5<<endl;
               
                    // cout<<"\nLeft eye: test my code @icog: "<<diffLeftEye<<endl;
                    // cout<<"\nRight eye: test my code @icog: "<<diffRightEye<<endl;

                    //vertices for right and left eye
                    long vsize = 0;
                    for (int vr = 36, vl = 42; vr<42, vl<48; vr++, vl++)
                    {
                        verticesRE[vsize][0] = lmPoints[vr][0];
                        verticesRE[vsize][1] = lmPoints[vr][1];

                        verticesLE[vsize][0] = lmPoints[vl][0];
                        verticesLE[vsize][1] = lmPoints[vl][1];

                        vsize++;
                    }
                    // https://www.youtube.com/watch?v=t9FzskM3P_0#t=58
                    // cout<<"\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
                    // cout<<"\nHello Outside-vsize "<<vsize<<" Right AREA:"<<areaOfShape(verticesRE, vsize);
                    // cout<<"\nHello Outside-vsize "<<vsize<<" Left AREA:"<<areaOfShape(verticesLE, vsize);
                    // cout<<"\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";

                    //vertices for Inner Opening
                    long msize = 0;
                    for (int m = 60; m<68; m++)
                    {
                        verticesIO[msize][0] = lmPoints[m][0];
                        verticesIO[msize][1] = lmPoints[m][1];
                        msize++;
                    }

                    //vertices for lipsSmileROut
                    lipsSmileR[0][0] = lmPoints[59][0];
                    lipsSmileR[0][1] = lmPoints[59][1];

                    lipsSmileR[1][0] = lmPoints[48][0];
                    lipsSmileR[1][1] = lmPoints[48][1];

                    lipsSmileR[2][0] = lmPoints[49][0];
                    lipsSmileR[2][1] = lmPoints[49][1];

                    lipsSmileR[3][0] = lmPoints[50][0];
                    lipsSmileR[3][1] = lmPoints[50][1];
                    
                    lipsSmileR[4][0] = lmPoints[60][0];
                    lipsSmileR[4][1] = lmPoints[60][1];

                    lipsSmileR[5][0] = lmPoints[61][0];
                    lipsSmileR[5][1] = lmPoints[61][1];

                    lipsSmileR[6][0] = lmPoints[67][0];
                    lipsSmileR[6][1] = lmPoints[67][1];

                    //vertices for lipsSmileLOut
                    lipsSmileL[0][0] = lmPoints[55][0];
                    lipsSmileL[0][1] = lmPoints[55][1];

                    lipsSmileL[1][0] = lmPoints[54][0];
                    lipsSmileL[1][1] = lmPoints[54][1];

                    lipsSmileL[2][0] = lmPoints[53][0];
                    lipsSmileL[2][1] = lmPoints[53][1];

                    lipsSmileL[3][0] = lmPoints[52][0];
                    lipsSmileL[3][1] = lmPoints[52][1];

                    lipsSmileL[4][0] = lmPoints[64][0];
                    lipsSmileL[4][1] = lmPoints[64][1];

                    lipsSmileL[5][0] = lmPoints[63][0];
                    lipsSmileL[5][1] = lmPoints[63][1];

                    lipsSmileL[6][0] = lmPoints[65][0];
                    lipsSmileL[6][1] = lmPoints[65][1];
                    //  //vertices for lipsSmileRInner
                    // lipsSmileRi[0][0] = lmPoints[60][0];
                    // lipsSmileRi[0][1] = lmPoints[60][1];

                    // lipsSmileRi[1][0] = lmPoints[61][0];
                    // lipsSmileRi[1][1] = lmPoints[61][1];

                    // lipsSmileRi[2][0] = lmPoints[67][0];
                    // lipsSmileRi[2][1] = lmPoints[67][1];

                     //vertices for lipsSmileRInner
                    // lipsSmileLi[0][0] = lmPoints[64][0];
                    // lipsSmileLi[0][1] = lmPoints[64][1];

                    // lipsSmileLi[1][0] = lmPoints[63][0];
                    // lipsSmileLi[1][1] = lmPoints[63][1];

                    // lipsSmileLi[2][0] = lmPoints[65][0];
                    // lipsSmileLi[2][1] = lmPoints[65][1];


                    cout<<"\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
                    // cout<<"\nlipsSmileR Nose @31: ("<<lmPoints[31][0]<<", "<<lmPoints[31][1]<<")";
                    double aR = areaOfShape(lipsSmileR, 7);
                    double aL = areaOfShape(lipsSmileL, 7);
                    // if(aR>aL)
                    // {
                        cout<<"\nArea of lipsSmileR: "<<aR;
                    // }
                    // else if (aL>aR)
                    // {
                    //     cout<<"\nArea of lipsSmileL: "<<aL;
                    // }
                    cout<<"\n@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@\n";
                    // cout<<"\nArea of lipsSmileL: "<<aL;
                    // cout<<"\nArea of lipsSmileR: "<<aR;
                    //start capturing first 100ds               
                    if (cnt < 100)
                    {
                        first100ds[cnt++] = aR;
                        cout<<"\ncnt:"<<cnt<<"\n";
                    }
                    else if (cnt == 100)
                    {
                        captured = true;
                        cout<<"\ncaptured the first "<<cnt<<" values\n";
                        cnt++;
                    }  
                     //here start code for finding average of captured
                    if(captured){
                        double sum = 0;
                        for (int i = 0; i < 100; ++i)
                        {
                            sum += first100ds[i];
                        }
                        first100dsAvg = sum/100;
                        cout<<"\n miiiiiiiiiiiiiiiiiiiiiiiiiiinnnnnnnnnnnn:"<<first100dsAvg<<"\n";
                        captured = false;                        
                        // cnt = 0;
                    }  

                    cur = aR;
                    if (cur > first100dsAvg && cur > max && cnt >= 100)
                    {
                        max = cur;
                        //here i have the maximum as well as the minimum so do the trick



                        icnt = 0;
                        cout<<"\n maxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx:"<<max<<"\n";

                    }else if (cur>first100dsAvg && cur < max && cnt >= 100)
                    {
                        //here push all the intermediate values to the array intermediateaR
                        //dont store but calculate
                        //normalize the cur value to 0-1 range i.e. if cur is in between (min, max) range
                        bv = (cur - first100dsAvg)/(max - first100dsAvg);
                        roundOff_bv = ceil( ( bv * pow( 10, 3) ) - 0.49 ) / pow( 10, 3);

                        intermediateaR[icnt] = cur;
                        cout<<"\nThe Blendeer value:"<<roundOff_bv<<endl;
                        icnt++;
                    }                   

                //     faceDetected = false;
            } else{
                cnt = 0;
                icnt = 0;
                // cout<<"\ncnt:"<<cnt<<endl;
            }   
            

            ///////////////////////////////////added
        }
        
        
    }
    catch(serialization_error& e)
    {
        cout << "You need dlib's default face landmarking model file to run this example." << endl;
        cout << "You can get it from the following URL: " << endl;
        cout << "http://dlib.net/files/shape_predictor_68_face_landmarks.dat.bz2" << endl;
        cout << endl << e.what() << endl;
    }
    catch(exception& e)
    {
        cout << e.what() << endl;
    }
    /*

    */

}
///////////////////////////////////added
double areaOfShape(double points[68][2], long rows){
    double sum = 0, area = 0;
    for(long r=0; r<rows; r++){
        if(r == rows-1){
            sum += points[r][0]*points[0][1] - points[0][0]*points[r][1];
        }else {
            sum += points[r][0]*points[r+1][1] - points[r+1][0]*points[r][1];
        }
    }
    area = abs((long)sum)/2;
    return area;
}
///////////////////////////////////added
/*
youtube-dl -f 140 https://www.youtube.com/watch?v=-dPh_KrrbNI&index=2&list=PLE8630C68E623AA4E && youtube-dl -f 140 https://www.youtube.com/watch?v=8-O5zKpp7s8&index=8&list=PLE8630C68E623AA4E && youtube-dl -f 140 https://www.youtube.com/watch?v=qOgoQjl_S1s&index=11&list=PLE8630C68E623AA4E && youtube-dl -f 140 https://www.youtube.com/watch?v=bQpVapkkD7w && youtube-dl -f 140 https://www.youtube.com/watch?v=3ei7RW4iD48&list=PLE8630C68E623AA4E&index=4

*/