#include <opencv2/opencv.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <vector>
#include <string>

struct float3 { float x, y, z; };
struct Face { int v1, v2, v3; };

// Function to load STL file using Assimp
bool loadSTL(const std::string& filename, std::vector<float3>& vertices, std::vector<Face>& faces) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate);

    if (!scene) {
        std::cerr << "Error loading STL file: " << importer.GetErrorString() << std::endl;
        return false;
    }

    for (int i = 0; i < scene->mNumMeshes; ++i) {
        aiMesh* mesh = scene->mMeshes[i];
        for (int j = 0; j < mesh->mNumVertices; ++j) {
            aiVector3D vertex = mesh->mVertices[j];
            vertices.push_back({ vertex.x, vertex.y, vertex.z });
        }
        for (int j = 0; j < mesh->mNumFaces; ++j) {
            aiFace face = mesh->mFaces[j];
            if (face.mNumIndices == 3) { // We only support triangles
                faces.push_back({ face.mIndices[0], face.mIndices[1], face.mIndices[2] });
            }
        }
    }

    return true;
}

// Defining the dimensions of checkerboard
int CHECKERBOARD[2]{ 5, 8 };

// Function to project 3D points to 2D
void projectSTLObject(const cv::Mat& cameraMatrix, const cv::Mat& distCoeffs, const cv::Mat& rvec, const cv::Mat& tvec, const std::vector<float3>& objectPoints, const std::vector<Face>& faces, cv::Mat& image) {
    std::vector<cv::Point3f> objectPointsCV;
    for (const auto& p : objectPoints) {
        objectPointsCV.push_back(cv::Point3f(p.x, p.y, p.z));
    }

    std::vector<cv::Point2f> imagePoints;
    cv::projectPoints(objectPointsCV, rvec, tvec, cameraMatrix, distCoeffs, imagePoints);

    for (const auto& face : faces) {
        cv::line(image, imagePoints[face.v1], imagePoints[face.v2], cv::Scalar(0, 255, 0), 1);
        cv::line(image, imagePoints[face.v2], imagePoints[face.v3], cv::Scalar(0, 255, 0), 1);
        cv::line(image, imagePoints[face.v3], imagePoints[face.v1], cv::Scalar(0, 255, 0), 1);
    }
}

int main() {
    // Load the 3D model
    std::vector<float3> objectPoints;
    std::vector<Face> faces;
    if (!loadSTL("../Malla.stl", objectPoints, faces)) {
        return -1;
    }

    // Creating vector to store vectors of 3D points for each checkerboard image
    std::vector<std::vector<cv::Point3f>> objpoints;

    // Creating vector to store vectors of 2D points for each checkerboard image
    std::vector<std::vector<cv::Point2f>> imgpoints;

    // Defining the world coordinates for 3D points
    std::vector<cv::Point3f> objp;
    for (int i{ 0 }; i < CHECKERBOARD[1]; i++) {
        for (int j{ 0 }; j < CHECKERBOARD[0]; j++)
            objp.push_back(cv::Point3f(j, i, 0));
    }

    // Extracting path of individual image stored in a given directory
    std::vector<cv::String> images;
    // Path of the folder containing checkerboard images
    std::string path = "../images/*.bmp";

    cv::glob(path, images);

    cv::Mat frame, gray;
    // vector to store the pixel coordinates of detected checker board corners 
    std::vector<cv::Point2f> corner_pts;
    bool success;

    // Looping over all the images in the directory
    for (int i{ 0 }; i < images.size(); i++) {
        frame = cv::imread(images[i]);
        cv::cvtColor(frame, gray, cv::COLOR_BGR2GRAY);

        // Finding checker board corners
        // If desired number of corners are found in the image then success = true  
        success = cv::findChessboardCorners(gray, cv::Size(CHECKERBOARD[0], CHECKERBOARD[1]), corner_pts, cv::CALIB_CB_ADAPTIVE_THRESH | cv::CALIB_CB_FAST_CHECK | cv::CALIB_CB_NORMALIZE_IMAGE);

        if (success) {
            cv::TermCriteria criteria(cv::TermCriteria::EPS | cv::TermCriteria::MAX_ITER, 30, 0.001);

            // refining pixel coordinates for given 2d points.
            cv::cornerSubPix(gray, corner_pts, cv::Size(11, 11), cv::Size(-1, -1), criteria);

            // Displaying the detected corner points on the checker board
            cv::drawChessboardCorners(frame, cv::Size(CHECKERBOARD[0], CHECKERBOARD[1]), corner_pts, success);

            objpoints.push_back(objp);
            imgpoints.push_back(corner_pts);
        }

        cv::imshow("Image", frame);
        cv::waitKey(0);
    }

    cv::destroyAllWindows();

    cv::Mat cameraMatrix, distCoeffs;
    std::vector<cv::Mat> rvecs, tvecs;

    // Performing camera calibration
    cv::calibrateCamera(objpoints, imgpoints, cv::Size(gray.rows, gray.cols), cameraMatrix, distCoeffs, rvecs, tvecs);

    std::cout << "cameraMatrix : " << cameraMatrix << std::endl;
    std::cout << "distCoeffs : " << distCoeffs << std::endl;

    // Looping over all the images in the directory again to project the 3D object
    for (int i{ 0 }; i < images.size(); i++) {
        frame = cv::imread(images[i]);

        if (i < rvecs.size() && i < tvecs.size()) {
            cv::Mat rvec = rvecs[i];
            cv::Mat tvec = tvecs[i];

            // Project and draw the 3D object points on the current image
            projectSTLObject(cameraMatrix, distCoeffs, rvec, tvec, objectPoints, faces, frame);
        }
        
        cv::imshow("Projected Image", frame);
        cv::waitKey(0);
    }

    return 0;
}
