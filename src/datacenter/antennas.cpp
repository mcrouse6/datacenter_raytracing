#include <iostream>
#include "antennas.h"

remcom::rxapi::RouteSetHandle createRowSet(int outID, Coord start, Coord end, float spacing)
{
    remcom::rxapi::RouteSetHandle row = remcom::rxapi::RouteSet::New( );
    row->setOutputID( outID );
    row->setConformToTerrain( true );

    remcom::rxapi::CartesianPointHandle rowStart = remcom::rxapi::CartesianPoint::New( );
    remcom::rxapi::CartesianPointHandle rowStop = remcom::rxapi::CartesianPoint::New( );

    rowStart->setX( start.x );
    rowStart->setY( start.y );
    rowStart->setZ( start.z );

    rowStop->setX( end.x );
    rowStop->setY( end.y );
    rowStop->setZ( end.z );
    
    // we can call setProjectedPoint( ) to set the first point in the list
    row->getControlPoints( )->setProjectedPoint( rowStart );

    // we need to call addProjectedPoint( ) to add subsequent points
    row->getControlPoints( )->addProjectedPoint( rowStop );

    // set the spacing
    row->setSpacing( spacing );
    return row;

}

remcom::rxapi::GridSetHandle createGridSet(int outID, Coord start, double x_len, double y_len, float spacing)
{
    remcom::rxapi::GridSetHandle grid = remcom::rxapi::GridSet::New( );
    grid->setOutputID( outID );
    grid->setConformToTerrain( true );

    remcom::rxapi::CartesianPointHandle rowStart = remcom::rxapi::CartesianPoint::New( );
    remcom::rxapi::CartesianPointHandle rowStop = remcom::rxapi::CartesianPoint::New( );

    rowStart->setX( start.x );
    rowStart->setY( start.y );
    rowStart->setZ( start.z );

    //rowStop->setX( end.x );
    //rowStop->setY( end.y );
    //rowStop->setZ( end.z );
    
    // we can call setProjectedPoint( ) to set the first point in the list
    grid->getControlPoints( )->setProjectedPoint( rowStart );

    // we need to call addProjectedPoint( ) to add subsequent points
    //row->getControlPoints( )->addProjectedPoint( rowStop );


    // we can call setProjectedPoint( ) to set the first point in the list
    grid->setLengthX(x_len);
    grid->setLengthY(y_len);

    // set the spacing
    grid->setSpacing( spacing );
    return grid;

}

remcom::rxapi::IsotropicHandle createIsotropicAntenna() 
{
    remcom::rxapi::IsotropicHandle iso = remcom::rxapi::Isotropic::New( );
    // create and assign the 908-5 MHz waveform
    remcom::rxapi::SinusoidHandle sinusoid = remcom::rxapi::Sinusoid::New( );
    sinusoid->setCarrierFrequency( 60.0e9 );  // frequency is specified in Hz
    //sinusoid->setCarrierFrequency( 2.4e9 ); // frequency is specified in Hz
    sinusoid->setBandwidth( 500.0 );

    iso->setWaveform( sinusoid );
    //iso->setGain(25.0);
    return iso;
 
}

remcom::rxapi::OmnidirectionalHandle createOmniAntenna()
{
    remcom::rxapi::OmnidirectionalHandle omni;
    /*
    remcom::rxapi::ShortDipoleHandle verticalDipole = remcom::rxapi::ShortDipole::New( );
    // create and assign the 908-5 MHz waveform
    remcom::rxapi::SinusoidHandle sinusoid = remcom::rxapi::Sinusoid::New( );
    sinusoid->setCarrierFrequency( 60.0e9 );  // frequency is specified in Hz
    sinusoid->setBandwidth( 500.0 );

    verticalDipole->setWaveform( sinusoid );
    return verticalDipole;
    */
    return omni;
 
}

remcom::rxapi::ShortDipoleHandle createAntenna() 
{
    remcom::rxapi::ShortDipoleHandle verticalDipole = remcom::rxapi::ShortDipole::New( );
    // create and assign the 908-5 MHz waveform
    remcom::rxapi::SinusoidHandle sinusoid = remcom::rxapi::Sinusoid::New( );
    sinusoid->setCarrierFrequency( 60.0e9 );  // frequency is specified in Hz
    sinusoid->setBandwidth( 500.0 );

    verticalDipole->setWaveform( sinusoid );
    return verticalDipole;
    
}

remcom::rxapi::HornHandle createHornAntenna() 
{
    remcom::rxapi::HornHandle horn = remcom::rxapi::Horn::New( );
    // create and assign the 908-5 MHz waveform
    remcom::rxapi::SinusoidHandle sinusoid = remcom::rxapi::Sinusoid::New( );
    sinusoid->setCarrierFrequency( 60.0e9 );  // frequency is specified in Hz
    sinusoid->setBandwidth( 1000.0 );

    horn->setWaveform( sinusoid );
    /* 
    Aperture Width : 0.0686 m
    Aperture Height: 0.0522 m
    Feed Length:  0.116 m
    Feed Width: 0.0073 m
    Feed Height: 0.0036 m
    */

    horn->setHornApertureWidth(0.0686);
    horn->setHornApertureHeight(0.0522);
    horn->setFeedApertureLength(0.116);
    horn->setFeedApertureHeight(0.0073);
    horn->setFeedApertureWidth(0.0036);
    horn->setGain(25.0);

    return horn;
    
}