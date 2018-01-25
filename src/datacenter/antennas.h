#include <rxapi/Node.h>
#include <rxapi/Primitive.h>
#include <rxapi/ProgressReporter.h>
#include <rxapi/Vector.h>

#include <rxapischema/CartesianPoint.h>
#include <rxapischema/GeometryList.h>
#include <rxapischema/City.h>
#include <rxapischema/DoubleOutputColumn.h>
#include <rxapischema/Job.h>
#include <rxapischema/OutputRequest.h>
#include <rxapischema/PointSet.h>
#include <rxapischema/PropagationPathRequest.h>
#include <rxapischema/Receiver.h>
#include <rxapischema/RouteSet.h>
#include <rxapischema/GridSet.h>
#include <rxapischema/Scene.h>
#include <rxapischema/ShortDipole.h>
#include <rxapischema/Omnidirectional.h>
#include <rxapischema/Isotropic.h>
#include <rxapischema/Horn.h>
#include <rxapischema/Sinusoid.h>
#include <rxapischema/Terrain.h>
#include <rxapischema/TxRxSetList.h>
#include <rxapischema/X3D.h>
#include <rxapischema/Directional.h>



struct Coord 
{
    float x;
    float y;
    float z; 
};

remcom::rxapi::RouteSetHandle createRowSet(int outID, Coord start, Coord end, float spacing);
remcom::rxapi::GridSetHandle createGridSet(int outID, Coord start, double x_len, double y_len, float spacing);

remcom::rxapi::ShortDipoleHandle createAntenna(); 
remcom::rxapi::HornHandle createHornAntenna(); 
remcom::rxapi::IsotropicHandle createIsotropicAntenna(); 
remcom::rxapi::OmnidirectionalHandle createOmniAntenna(); 
remcom::rxapi::DirectionalHandle createDirectionalAntenna() ;

