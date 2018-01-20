#include <iostream>
#include <geometry.h>

remcom::rxapi::CityHandle
addCity( remcom::rxapi::GeometryListHandle& geometryList, 
         remcom::rxapi::RString const& filename )
{
    // create a new City node.  This is a "Geometry"
    remcom::rxapi::CityHandle city = remcom::rxapi::City::New( );

    // Make the city's GeometrySource, which we know is a WirelessInSiteGeometry because it
	// is a city.
	remcom::rxapi::WirelessInSiteGeometryHandle gSource = 
						remcom::rxapi::WirelessInSiteGeometry::New();

	// Put that source into the city
	city->setGeometrySource( gSource );

	// From the GeometrySource, get the filename node and then set its value to our filename
    gSource->getFilename( )->setFilename( filename );

    // add the new node to the geometry list and then return the updated list
    geometryList->addGeometry( city );
    return city;
}

/*
remcom::rxapi::ObjectHandle
addObject( remcom::rxapi::GeometryListHandle& geometryList, 
           remcom::rxapi::RString const& filename )
{
    // create a new Object node
    remcom::rxapi::ObjectHandle object = remcom::rxapi::Object::New( );

    // first get the new node's filename child (which is a FileDescription)
    // and then set its filename child to the specified filename
    object->getFilename( )->setFilename( filename );

    // add the new node to the geometry list and then return the updated list
    geometryList->addGeometry( object );
    return object;
}
*/
remcom::rxapi::ShapefileHandle 
addShapefile( remcom::rxapi::GeometryListHandle& geometryList, 
              remcom::rxapi::RString const& filename, 
              remcom::rxapi::RString const& heightAttribute )
{
    // create a new Shapefile node.  Unlike City files, which are geometries,
	// this is a GeometrySource
    remcom::rxapi::ShapefileHandle shapefile = remcom::rxapi::Shapefile::New( );

    // first get the new node's filename child (which is a FileDescription)
    // and then set its filename child to the specified filename
    {
        // option 1: get a handle to the Filename child node, then get a handle
        // to that node's Filename child, and then call setValue( ... ), passing
        // in the filename
        shapefile->getFilename( )->getFilename( )->setValue( filename );
    }
    {
        // option 2: get a handle to the Filename child node and then use the 
        // setFilename( ... ) helper function provided in FileDescription.h
        shapefile->getFilename( )->setFilename( filename );
    }

    // set the height attribute
    {
        // option 1: get a handle to the HeightAttribute child node and then 
        // call setValue( ... ), passing in the height attribute
        shapefile->getHeightAttribute( )->setValue( heightAttribute );
    }
    {
        // option 2: use the utility function setHeightAttribute( ... ) provided
        // in Shapefile.h
        shapefile->setHeightAttribute( heightAttribute );
    }

	// Make a geometry node, and put the shapefile GeometrySource into it.
	remcom::rxapi::ObjectHandle h  = remcom::rxapi::Object::New();
	remcom::rxapi::GeometryHandle gH = h;
	gH->setGeometrySource( shapefile );

    // add the new node to the geometry list and then return the updated list
    geometryList->addGeometry( gH );
    return shapefile;
}

remcom::rxapi::COLLADAHandle
addColladaFile( remcom::rxapi::GeometryListHandle& geometryList, 
              remcom::rxapi::RString const& filename)
{
    // create a new Shapefile node.  Unlike City files, which are geometries,
	// this is a GeometrySource
    remcom::rxapi::COLLADAHandle shapefile = remcom::rxapi::COLLADA::New( );

    // first get the new node's filename child (which is a FileDescription)
    // and then set its filename child to the specified filename
    {
        // option 1: get a handle to the Filename child node, then get a handle
        // to that node's Filename child, and then call setValue( ... ), passing
        // in the filename
        shapefile->getFilename( )->getFilename( )->setValue( filename );
    }
    {
        // option 2: get a handle to the Filename child node and then use the 
        // setFilename( ... ) helper function provided in FileDescription.h
        shapefile->getFilename( )->setFilename( filename );
    }




	// Make a geometry node, and put the shapefile GeometrySource into it.
	remcom::rxapi::ObjectHandle h  = remcom::rxapi::Object::New();
	remcom::rxapi::GeometryHandle gH = h;
	gH->setGeometrySource( shapefile );

    // add the new node to the geometry list and then return the updated list
    geometryList->addGeometry( gH );
    return shapefile;
}

remcom::rxapi::ShapefileHandle 
addShapefile( remcom::rxapi::GeometryListHandle& geometryList, 
              remcom::rxapi::RString const& filename, 
              remcom::rxapi::RString const& heightAttribute,
              remcom::rxapi::MaterialHandle defaultMaterial )
{
    // call the simpler addShapefile( ... ) function first
    remcom::rxapi::ShapefileHandle shapefile = addShapefile( geometryList, filename, heightAttribute );

    // now set the default material which will override the "default default"
    // dry concrete material usually applied to all facets

    // first, check to see if we already have an (optional) default material child node
    if( shapefile->firstChild( "DefaultMaterial" ) != shapefile->end( ) )
    {
        // option 1: we already have a default material set, so change it
        shapefile->setDefaultMaterial( defaultMaterial );
    }
    else
    {
        // option 2: we do not yet have a default material set, so add it
        shapefile->addChild( "DefaultMaterial", defaultMaterial );
    }

    return shapefile;
}

remcom::rxapi::TerrainHandle 
addTerrain( remcom::rxapi::GeometryListHandle& geometryList, 
            remcom::rxapi::RString const& filename )
{
    // create a new Terrain node
    remcom::rxapi::TerrainHandle terrain = remcom::rxapi::Terrain::New( );

	// Make the terrain's GeometrySource, which we know is a WirelessInSiteGeometry because it
	// is a terrain.
	remcom::rxapi::WirelessInSiteGeometryHandle gSource = 
		remcom::rxapi::WirelessInSiteGeometry::New();

	// Put that source into the terrain
	terrain->setGeometrySource( gSource );

    // first get the new node's filename child (which is a FileDescription)
    // and then set its filename child to the specified filename
    gSource->getFilename( )->setFilename( filename );

    // add the new node to the geometry list and then return the updated list
    geometryList->addGeometry( terrain );
    return terrain;
}
