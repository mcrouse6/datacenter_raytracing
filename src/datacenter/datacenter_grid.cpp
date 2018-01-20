#include <math.h>
#include <geometry.h>
#include <output.h>
#include "antennas.h"
#include <vector>

#include <rxapi/Node.h>
#include <rxapi/Primitive.h>
#include <rxapi/ProgressReporter.h>
#include <rxapi/Vector.h>

#include <rxapischema/CartesianPoint.h>
#include <rxapischema/GeometryList.h>
#include <rxapischema/COLLADA.h>
#include <rxapischema/City.h>
#include <rxapischema/DoubleOutputColumn.h>
#include <rxapischema/Job.h>
#include <rxapischema/OutputRequest.h>
#include <rxapischema/PointSet.h>
#include <rxapischema/PropagationPathRequest.h>
#include <rxapischema/Receiver.h>
#include <rxapischema/RouteSet.h>
#include <rxapischema/Scene.h>
#include <rxapischema/ShortDipole.h>
#include <rxapischema/Sinusoid.h>
#include <rxapischema/Terrain.h>
#include <rxapischema/TxRxSetList.h>
#include <rxapischema/X3D.h>

#ifdef _WIN32
    #define DATA_DIR "..\\..\\..\\..\\data\\"
#else
    // for Linux distributions, please configure DATA_DIR as necessary
    #define DATA_DIR "../../data/"
#endif
#define RESULTS_DIR "../../results/"

static const double originLon = 0;
static const double originLat = 0;

#define PI 3.14159265

#define RACK_CENTER_X .292
#define RACK_CENTER_Y .533
#define RACK_X_SPACING .6
#define RACK_Y_SPACING 2.4384
#define ANTENNA_HEIGHT 2.3

#define TX_T 1
#define RX_T 2

using namespace std;

void printPoint(remcom::rxapi::CartesianPointHandle p)
{
    printf("%f, %f, %f\n", p->getX()->getValue(), p->getY()->getValue(), p->getZ()->getValue());

}

remcom::rxapi::PointSetHandle createNode(double x, double y, double z, int id)
{
    // add transmitters/receivers
    // add Tx 2B from the Wireless InSite project
    remcom::rxapi::PointSetHandle node = remcom::rxapi::PointSet::New( );
    node->setOutputID( id );
    
    // create antenna - currently just Dipole
    // TODO: make more general and pass params
    //remcom::rxapi::ShortDipoleHandle horn = createAntenna();
    // remcom::rxapi::HornHandle horn = createHornAntenna();
    //remcom::rxapi::IsotropicHandle horn = createIsotropicAntenna();

    // set Tx 2B's control point
    remcom::rxapi::CartesianPointHandle point = remcom::rxapi::CartesianPoint::New( );
    point->setX( x );
    point->setY( y );
    point->setZ( z );

    // printPoint(point);

    node->getControlPoints( )->setProjectedPoint( point );

    return node;
}



void setAntennaType(remcom::rxapi::PointSetHandle node, int type)
{

    // remcom::rxapi::IsotropicHandle iso = createIsotropicAntenna();
    remcom::rxapi::HornHandle horn = createHornAntenna();

    // create the receiver properties
    if(type == RX_T)
    {
        remcom::rxapi::ReceiverHandle properties = remcom::rxapi::Receiver::New( );
        properties->setAntenna( horn );



        node->setReceiver( properties );

    }
    else if(type == TX_T)
    {
        remcom::rxapi::TransmitterHandle properties = remcom::rxapi::Transmitter::New( );
        properties->setAntenna( horn );
        properties->setInputPower(0.0);

        node->setTransmitter( properties );
    }
    else
    {
        cout << "antenna type unknown " << type << endl;
        exit(5);
    }
    
}


void setTxAngle(remcom::rxapi::PointSetHandle node, double rx, double ry, double rz)
{        // zero out the antenna rotations

    node->getTransmitter()->getAntennaRotations( )->setX( remcom::rxapi::Vector< double >( 1, rx ) );
    node->getTransmitter()->getAntennaRotations( )->setY( remcom::rxapi::Vector< double >( 1, ry ) );
    node->getTransmitter()->getAntennaRotations( )->setZ( remcom::rxapi::Vector< double >( 1, rz ) );

}

void setRxAngle(remcom::rxapi::PointSetHandle node, double rx, double ry, double rz)
{        // zero out the antenna rotations

    node->getReceiver()->getAntennaRotations( )->setX( remcom::rxapi::Vector< double >( 1, rx ) );
    node->getReceiver()->getAntennaRotations( )->setY( remcom::rxapi::Vector< double >( 1, ry ) );
    node->getReceiver()->getAntennaRotations( )->setZ( remcom::rxapi::Vector< double >( 1, rz ) );

}

remcom::rxapi::CartesianPointHandle getPosition(remcom::rxapi::PointSetHandle node)
{
    remcom::rxapi::ProjectedPointHandle proj_point = node->getControlPoints()->getProjectedPoint();
    return remcom::rxapi::Handle_cast< remcom::rxapi::CartesianPointHandle>( proj_point );
}

double calculateAngle(remcom::rxapi::PointSetHandle x_node, remcom::rxapi::PointSetHandle y_node)
{
    remcom::rxapi::CartesianPointHandle x_pos = getPosition(x_node);
    remcom::rxapi::CartesianPointHandle y_pos = getPosition(y_node);
    double dx = x_pos->getX()->getValue() - x_pos->getX()->getValue();
    double dy = y_pos->getY()->getValue() - y_pos->getY()->getValue();

    double dz = atan2(dy, dx) * 180.0 / PI;
    return dz;
}

// Apparently, WI needs to rotate around y instead of z axis for this to work
void pointRxAtTx(remcom::rxapi::PointSetHandle x_node, remcom::rxapi::PointSetHandle y_node)
{
    double dz =  calculateAngle(x_node, y_node);
    setRxAngle(x_node, 0, dz + 90.0, 0 );
}

// Apparently, WI needs to rotate around y instead of z axis for this to work
void pointTxAtRx(remcom::rxapi::PointSetHandle x_node, remcom::rxapi::PointSetHandle y_node)
{
    double dz =  calculateAngle(x_node, y_node);
    setTxAngle(x_node, 0, dz + 90.0, 0 ); 
}

int main( int argc, char** argv )
{
    static const int num_racks_per_row = 4;
    static const int num_rows = 4;

    // a progress reporter must be instantiated and registered with the rxapi to provide error, warning, and progress messages
    remcom::rxapi::ProgressReporter progressReporter( NULL );
    remcom::rxapi::Factory::instance( ).setProgressReporter( progressReporter );

    // create the scene
    remcom::rxapi::SceneHandle scene = remcom::rxapi::Scene::New( );

    // set the scene's origin to match the Wireless InSite project's origin
    scene->getOrigin( )->setLongitude( originLon );
    scene->getOrigin( )->setLatitude( originLat );

    // create geometry // add the rosslyn city - this is the city node itself, and we will assign its source next
    remcom::rxapi::GeometryListHandle gL = remcom::rxapi::GeometryList::New( );

    remcom::rxapi::COLLADAHandle shape = addColladaFile(gL, remcom::rxapi::RString( std::string( DATA_DIR ) + "DataCenter_FS_6x64.dae" )) ;
    scene->setGeometryList(gL);


    // add transmitters/receivers
    // add Tx 2B from the Wireless InSite project
    remcom::rxapi::PointSetHandle tx_node;
    tx_node = createNode(0.292000, -0.533000, ANTENNA_HEIGHT, 100);

    //figure out how to set alignment between antennas
    setAntennaType(tx_node, TX_T);
    setTxAngle(tx_node, 0,-90.0,0);

    scene->getTxRxSetList( )->addTxRxSet( tx_node );

    vector<remcom::rxapi::PointSetHandle> node_list;

    for(int j = 0; j< num_rows; j++) {
        for(int i = 0; i < num_racks_per_row; i++) {
            remcom::rxapi::PointSetHandle node;
            node = createNode((RACK_X_SPACING*i)+RACK_CENTER_X, 
                              ((RACK_Y_SPACING*j)+RACK_CENTER_Y)*-1.0,  
                              ANTENNA_HEIGHT, 
                              ((j*num_racks_per_row)+i));

            cout << (j*num_racks_per_row)+i << endl;;

            setAntennaType(node,  RX_T);
            setRxAngle(node, 0, 0, 0.0);
            pointRxAtTx(node, tx_node);
            scene->getTxRxSetList( )->addTxRxSet( node );
            node_list.push_back(node);
        }
    }
    // return 0;


    // create an x3d study area
    remcom::rxapi::X3DHandle x3d = remcom::rxapi::X3D::New( );
    x3d->setUseGPU(false);
    

    // set the model parameters to something reasonably balanced between fidelity and speed
    x3d->setReflections( 8 );
    x3d->setTransmissions( 0 );
    x3d->setDiffractions( 1 );
    x3d->setCPUThreads( 16 );

    // now that we have a full scene and a model to run, create a job and hook everything together
    remcom::rxapi::JobHandle job = remcom::rxapi::Job::New( );
    job->setScene( scene );
    job->setModel( x3d );

    // the x3d model uses an SQLite database to store its paths
    remcom::rxapi::PathResultsDatabaseHandle database = remcom::rxapi::PathResultsDatabase::New( );
    job->setPathResultsDatabase( database );
    remcom::rxapi::FileDescriptionHandle databaseFilename = remcom::rxapi::FileDescription::New( );
    databaseFilename->setFilename( std::string(RESULTS_DIR) + "rosslyn_streets.sql" );
    database->setFilename( databaseFilename );

    // all requirements for running an x3d calculation should now be met, so check that the job is valid and then run
    if( !job->isValid( ) )
    {
        progressReporter.reportError( job->getReasonWhyInvalid( ) );
        return 1;
    }

    // at this point, the job is ready to run; it may be beneficial to serialize it to XML for viewing
    remcom::rxapi::Factory::instance( ).save( job, std::string(RESULTS_DIR) + "rosslyn_streets.xml", false );

    job->execute( &progressReporter );

    // once the job has finished, it will be updated with output, which can also be serialized
    remcom::rxapi::Factory::instance( ).save( job, std::string(RESULTS_DIR) + "rosslyn_streets_out.xml", false );

    if( !(job->getOutput( )->isValid( )) )
    {
        progressReporter.reportError( job->getOutput( )->getReasonWhyInvalid( ) );
        return 1;
    }

    // write output to the console
    //writeAllChannelsToConsole( database, tx_node->getOutputID( )->getValue( ), node->getOutputID( )->getValue( ) );
    for(int i = 0; i < node_list.size(); i++) {
        writeAllChannelsToConsole( database, tx_node->getOutputID( )->getValue( ), node_list[i]->getOutputID( )->getValue( ) );
    }
    //writeAllChannelsToConsole( database, row->getOutputID( )->getValue( ), tx2b->getOutputID( )->getValue( ) );

    return 0;
}
