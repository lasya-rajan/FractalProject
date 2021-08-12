#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Camera.h"
#include "cinder/params/Params.h"
#include "cinder/Easing.h"
#include "cinder/Rand.h"
#include "cinder/ImageIo.h"
#include <vector>

#include "Resources.h"


using namespace ci;
using namespace ci::app;
using namespace std;
using std::vector;

GLfloat mat_diffuse[]        = { 0.3, 0.5, 0.8, 1.0 };
GLfloat light1_diffuse[]    = { 1.0, 1.0, 1.0, 1.0 };

class FractalProjectApp : public AppBase {
  public:
    void prepareSettings( Settings *settings );
    void setup();
    void mouseDown( MouseEvent event );
    void keyDown( KeyEvent event );
    void update();
    void draw();
    
    vec3 chaosGamePoint( vec3 point );
    
    // PARAMS
    params::InterfaceGl    mParams;
    
    // CAMERA
    CameraPersp mCam;
    quat mSceneRotation;
    float    mCameraDistance;
    vec3  mEye, mCenter, mUp;
    
    float mDirectional;
    
    // MARKERS
    vec3 marker1, marker2, marker3, marker4;
    
    vector<vec3>    mPoints;
    
    vec3 thePoint;
    
    bool mSaveFrame;
    
};

void FractalProjectApp::prepareSettings( Settings *settings )
{
    settings->setWindowSize( 1000, 800 );
    settings->setFrameRate( 60.0f );
}

void FractalProjectApp::setup()
{
    
    // SETUP CAMERA
    mCameraDistance = 1000.0f;
    mEye            = vec3( 0.0f, 300.0f, mCameraDistance );
    mCenter            = vec3( 0.0f, 300.0f, 0.0f );
    mUp                = vec3(yAxis());
    mCam.setPerspective( 75.0f, getWindowAspectRatio(), 5.0f, 3000.0f );
    
    
    //PARAMS
    mParams = params::InterfaceGl( "Control Panel", vec2( 200, 160 ) );
    mParams.addParam( "Scene Rotation", &mSceneRotation );
    mParams.addParam( "Eye Distance", &mCameraDistance, "min=50.0 max=1300.0 step=20.0 keyIncr=s keyDecr=w" );
    
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    
    gl::enableDepthWrite();
    gl::enableDepthRead();
    gl::enableAlphaBlending();
    
    glDisable( GL_TEXTURE_2D );
    
    mDirectional = 1.0f;
    
    marker1 = vec3(0.0f, 0.0f, 400.0f);
    marker2 = vec3( -346.5f, 0.0f, -200.0f);
    marker3 = vec3(346.5f, 0.0f, -200.0f);
    marker4 = vec3( 0.0f, 600.0f, 0.0f);
    
    thePoint = vec3( Rand::randFloat(-100.0f, 100.0f), Rand::randFloat(-100.0f, 100.0f), Rand::randFloat(-100.0f, 100.0f) );
    
    mSaveFrame = true;
}

void FractalProjectApp::mouseDown( MouseEvent event )
{
}

void FractalProjectApp::keyDown( KeyEvent event )
{
    if( event.getChar() == 's' ){
        mSaveFrame = ! mSaveFrame;
    }
}

void FractalProjectApp::update()
{
    //UPDATE CAMERA
    mEye = vec3( 0.0f, 0.0f, mCameraDistance);
    
    // mEye = Vec3f( 800.0f * sin(camAngle), 0.0f, 800.0f * cos( camAngle ) );
    mCam.lookAt( mEye, mCenter, mUp );
    gl::setMatrices( mCam );
    gl::rotate( mSceneRotation );
    
    thePoint = chaosGamePoint( thePoint );

}

void FractalProjectApp::draw()
{

    
    ci::ColorA color1( CM_RGB, 1.0f, 1.0f, 1.0f, 1.0f );
    glMaterialfv( GL_FRONT, GL_DIFFUSE,    color1 );
    
    
    gl::drawSphere( marker1, 10 );
    gl::drawSphere( marker2, 10 );
    gl::drawSphere( marker3, 10 );
    gl::drawSphere( marker4, 10 );
    
    for (int i = 0; i < mPoints.size(); i++ ) {
        
        ci::ColorA color2( CM_RGB, abs(mPoints[i].x/300.0f), mPoints[i].y/600, 1.0f, 1.0f );
        glMaterialfv( GL_FRONT, GL_DIFFUSE,    color2 );
        
        gl::drawSphere( mPoints[i], 1.0f );
    }
    
    
    if( mSaveFrame ){
        writeImage( getHomeDirectory() + "Sierpinski3D/sierpinski_" + toString( getElapsedFrames() ) + ".png", copyWindowSurface() );
        
       //mSaveFrame = false;
    }
    
    // DRAW PARAMS WINDOW
    params::InterfaceGl::draw();
    
}

vec3 FractalProjectApp::chaosGamePoint( Vec3f point )
{
    Vec3f midPoint;
    Vec3f chosenMarker;
    
    int markerNumber;
    
    markerNumber = Rand::randInt( 1, 5 );
    
    switch (markerNumber) {
        case 1:
            chosenMarker = marker1;
            break;
        case 2:
            chosenMarker = marker2;
            break;
        case 3:
            chosenMarker = marker3;
            break;
        case 4:
            chosenMarker = marker4;
            break;
    }
    
    midPoint.set( (point.x + chosenMarker.x)/2, (point.y + chosenMarker.y)/2, (point.z + chosenMarker.z)/2 );
    mPoints.push_back( midPoint );
    
    return midPoint;
    
    
}

CINDER_APP_BASIC( FractalProjectApp, RendererGl )
