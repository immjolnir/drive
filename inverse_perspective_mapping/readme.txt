
- https://marcosnietoblog.wordpress.com/2014/02/22/source-code-inverse-perspective-mapping-c-opencv/

It is a C++ class to compute Inverse Perspective Mappings (IPM), or sometimes called bird’s-eye views of a planar surface.

It is nothing else than a plane-to-plane homography, but in my experience it is not that easy to compute. I am using here OpenCV remap functions.

In the example I have hard-coded the necessary four-point correspondences required to compute the homography, but you can compute then as you want. You might use vanishing points, camera calibration information, or any other source.

Of course this type of classes can get really useful when you add them information about the camera calibration.

> This tutorial shows only how to create warped images. To compute distances you need to calibrate the camera and determine its R and t with respecto to the plane; or perhaps you can just get metric references in your scene and apply scale factor in x and y to the birds-eye views.

> The example I provide uses the OpenCV’s remap function, which is way faster than warpPerspective since it pre-computes all the projections that fill the destination image and store the indexes in a x-map and a y-map.

> The OpenCV warpPerspective function is a function that creates a warp from the scratch, converting each position of a plane into another position using the homography information. Of course, this is very intense computation for images, since each pixel need to be transformed.
The remap function computes a look-up table, that basically takes the same time as warpPerspective, but whose result can be stored for reuse in subsequent images. You gain time because access to memory is much faster than the projective equations.
However, of course, if your homography is changing, you can not keep the result of the remap function! Because you can not take advantage of precomputations done in the past.

In your application, it is absolutely required to map into the object, therefore you need to go through this step. You have maybe two main options:
1)Using OpenGL instead of OpenCV, because it is specially designed to render 3D entities and creating warps. I am unsure if your HW does have support for OpenGL or has GPU capabilities, but definitely it is worth to investigate the question.
2)If you have good programming skills, you can go for optimize the code, unwrap loops, use pointer arithmetic and use carefully the registers of your machine by dissasembling the code.


> Well, obviously, you are using the default 4-point correspondences I used for my case (which were hardcoded to keep the sample code easy and simple). For your case “C:/176955961.jpg”, you will need to determine how these correspondences are.
The IPM code assumes you’ve got this info. Methods for getting this can include vanishing point estimation, which is another one main topic in the blog.

Q: I’m currently using your IPM library to estimate the distance of a vehicle seen by a facing forward camera. I have the intrinsic camera calibration parameters and the camera’s pitch angle is computed using vanish point estimation.

The IPM works well. But my IPM image is strangely bouncing and I don’t know why. This leads me to estimate an imprecise distance to the front vehicle, because the same seems to goes foward and backward as the camera moves. Is this related to the pitch angle estimate? Can you point me a solution?

A: This is a well-known problem of IPM. It is assuming that the road is a plane. Obviously, real roads are not. They have vertical curvature, therefore the plane-to-plane mapping introduce distortion. Additionally, the camera is never moving perfectly straight and parallel to the road plane. There are vibrations from the car, suspension, and also acceleration and deceleration effects. In summary, the IPM can only hope to have an approximate parallel view of the road, but an arbitrary amount of distortion and variability.
The distance to the other vehicles must be computed using filtering techniques, such as Kalman filter, that absorb (part of) this variability and produce smooth results. This is a challenge and not yet solved by the scientific community in an universal manner.


Q: I looked at your IPM code, it requires four points at the source image and four corresponding points to do the job. But my project requires to using camera intrinsic matrix and transformation matrix. How can I do it? overload the getPerspectiveTransform() with my code for calculating homography matrix?
Regards

A: If you do have full calibration, you’ve got all the information you need to project from 3D to 2D. Also, you can create bird’s-eye views of the road, if this is your goal.
Instead of chosing 4 points manually, you can select them on the 3D world, and project them into the image using your projection matrix. Then, you can create the warp.

Q: HI Marcos thanks for your hard work. Im carrying out a research project regarding lane detection on smartphone cameras. I would like to extract an IPM of just the Lane in front of the camera including 25% either side of the lane. I have my lane detection working but I am trying to implement inverse perspective mapping. I have my camera projection matrix how do I use this to generate the correct 4 point correspondences and the correct homography matrix to get a birds eye view of the road?



A: The inverse perspective mapping can be thought as an homography between 4 points in the image, and 4 points in the world plane. Therefore, if you are able to locate a vanishing point, you can determine 4 points in the image that corresponds to a 4-point rectangular shape in the world (road) plane.
If you want to extend horizontally your view of the road, you only need to extend the 4-points in the road, and get the corresponding 4 points in the image by using the homography.

The vanishing point is directly related to a direction in 3D. Therefore, given the instrinsic parameters, you can known a 3D vector from a 2D point or viceversa.

Q: Hello,thanks for your codes.I am a student and very interested in your algorithms.While,could you tell me how did you choose the four origin points manually? Did you have some tricks or other experience? When I use your codes in my video,it doesn’t work .

A: You hitted the point. Selecting the right 4-points is the real difficulty. Actually, in the examples I use some hard-coded points that fit well with the sample video. However, for any other video with a different perspective, you will need to find/define them somehow.
My approach in the case of not having the extrinsics is normally the following:
1) Detect the vanishing point (or at least the horizon) by using Hough or some similar line-detection algorithm.
2) Draw a triangle from the vanishing point to the bottom left and right corners of the image.
3) Select an appropriate row below the vanishing point and find the intersection with the triangle sides.
A full set of other approaches can be found in my PhD dissertation (http://oa.upm.es/4837/1/MARCOS_NIETO_DONCEL.pdf).

Q: I am trying to implement IPM (and the project) described in the paper:

Danescu R, Itu R, Petrovai A. Generic dynamic environment perception using smart mobile devices. Sensors. 2016 Oct 17;16(10):1721.

My understanding is that there are two alternatives to hard coding 4 correspondance points manually:
1) Vanishing point method
2) Camera calibration

Can we rely on vanishing point method if the camera position and orientation does not change throughout the application? Is camera calibration more reliable, though more difficult?


A: I’ve not read the work you mention, but the question about IPM is as follows:
IPM is essentially a method which transforms an input image into another image by using a 3×3 homography. This is only accurate in the context of roads, if the road is actually flat (a plane), and the exact translation and rotation between the image plane (the virtual plane defined by the pinhole camera) and the road plane is computed.

By default, the problem can’t be solved with a hard-coded solution, because the road is never perfectly flat, and the vehicle itself has dynamics (e.g. suspension, vibration, inertia) which makes a fixed homography don’t map well at all times.

The solution is either to (a) compute the vanishing point or other image-based features (e.g. point correspondences) at every frame to dynamically update the homography, or (b) don’t update an IPM, and assume your reconstruction is incorrect at all times, and you need to model this error and incorporate that into your model (e.g. your parabola computing model).

I’ve tried both alternatives several times, and now I am more satisfied with the second one, basically because I skip some processing steps and the overall solution is faster.
