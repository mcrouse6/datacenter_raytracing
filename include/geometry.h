#include <rxapischema/City.h>
#include <rxapischema/GeometryList.h>
#include <rxapischema/Geometry.h>
#include <rxapischema/GeometrySource.h>
#include <rxapischema/COLLADA.h>
#include <rxapischema/WirelessInSiteGeometry.h>

#include <rxapischema/Object.h>
#include <rxapischema/Shapefile.h>
#include <rxapischema/Terrain.h>

/*!
    \brief Add a Wireless InSite city to the specified geometry list.

    \param geometryList The GeometryList node to which to add the city.
    \param filename The filename of the Wireless InSite city.

    \return Returns a handle to the new City node.
*/
remcom::rxapi::CityHandle addCity( remcom::rxapi::GeometryListHandle& geometryList, 
                                   remcom::rxapi::RString const& filename );

/*!
    \brief Add a Wireless InSite object to the specified geometry list.

    \param geometryList The GeometryList node to which to add the object.
    \param filename The filename of the Wireless InSite object.

    \return Returns a handle to the new Object node.
*/
remcom::rxapi::ObjectHandle addObject( remcom::rxapi::GeometryListHandle& geometryList, 
                                       remcom::rxapi::RString const& filename );

/*!
    \brief Add a shapefile to the specified geometry list.
    
    \param geometryList The GeometryList node to which to add the shapefile.
    \param filename The filename of the shapefile. This must end in ".shp".
    \param heightAttribute The height attribute in the file which specified building heights.
    
    \return Returns a handle to the new Shapefile node.
*/
remcom::rxapi::ShapefileHandle addShapefile( remcom::rxapi::GeometryListHandle& geometryList, 
                                             remcom::rxapi::RString const& filename, 
                                             remcom::rxapi::RString const& heightAttribute );


remcom::rxapi::COLLADAHandle
addColladaFile( remcom::rxapi::GeometryListHandle& geometryList, 
              remcom::rxapi::RString const& filename);

/*!
    \brief Add a shapefile to the specified geometry list.
    
    \param geometryList The GeometryList node to which to add the shapefile.
    \param filename The filename of the shapefile. This must end in ".shp".
    \param heightAttribute The height attribute in the file which specified building heights.
    \param defaultMaterial The default material to use. This material will be applied to all
    facets contained in the shapefile and overrides the "default default" dry concrete material.
    
    \return Returns a handle to the new Shapefile node.
*/
remcom::rxapi::ShapefileHandle addShapefile( remcom::rxapi::GeometryListHandle& geometryList, 
                                             remcom::rxapi::RString const& filename, 
                                             remcom::rxapi::RString const& heightAttribute,
                                             remcom::rxapi::MaterialHandle defaultMaterial );

/*!
    \brief Add a Wireless InSite terrain to the specified geometry list.

    \param geometryList The GeometryList node to which to add the terrain.
    \param filename The filename of the Wireless InSite terrain.

    \return Returns a handle to the new Terrain node.
*/
remcom::rxapi::TerrainHandle addTerrain( remcom::rxapi::GeometryListHandle& geometryList, 
                                         remcom::rxapi::RString const& filename );
