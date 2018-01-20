#include <fstream>
#include <iostream>

#include <rxapi/ProgressReporter.h>

#include <rxapischema/GetChannelJob.h>
#include <rxapischema/GetPathsJob.h>
#include <rxapischema/Output.h>

#include <output.h>

void
writeAllChannelsToConsole( remcom::rxapi::PathResultsDatabaseHandle database, int txSet, int rxSet )
{
    remcom::rxapi::GetChannelJobHandle channel = remcom::rxapi::GetChannelJob::New( );

    // by setting just the tx and rx set ids, we request all results for all channels in those sets
    channel->setPathResultsDatabase( database );
    channel->setTxSetID( txSet );
    channel->setRxSetID( rxSet );

    if( !channel->isValid( ) )
    {
        std::cout << "Channel is invalid." << std::endl;
        return;
    }

    channel->execute( );

    // now we iterate over the list of ChannelData elements in the result
    remcom::rxapi::ChannelListResultHandle result = channel->getResult( );
    
    remcom::rxapi::ChannelData::Iterator channelDataIter;
    remcom::rxapi::ChannelDataHandle channelData;

    remcom::rxapi::ChannelEMListHandle emList;

    remcom::rxapi::ChannelEMData::Iterator emDataIter;
    remcom::rxapi::ChannelEMDataHandle emData;
    
    channelDataIter = result->firstChild( "Channel" );
    while( channelDataIter != result->end( ) )
    {
        remcom::rxapi::NodeHandle channelDataNode = channelDataIter.getNode( );
        channelData = remcom::rxapi::Handle_cast< remcom::rxapi::ChannelDataHandle >( channelDataNode );

        std::cout << "Tx Set: " << channelData->getTxSetID( )->getValue( ) << std::endl;
        std::cout << "Tx Point (Global): " << channelData->getTxPointID( )->getValue( ) << std::endl;
        std::cout << "Rx Set: " << channelData->getRxSetID( )->getValue( ) << std::endl;
        std::cout << "Rx Point (Global): " << channelData->getRxPointID( )->getValue( ) << std::endl;
        std::cout << "Channel (Global): " << channelData->getChannelID( )->getValue( ) << std::endl;

        emList = channelData->getChannelEMList( );
        
        emDataIter = emList->firstChild( "ChannelEMData" );
        remcom::rxapi::NodeHandle emDataNode = emDataIter.getNode( );
        emData = remcom::rxapi::Handle_cast< remcom::rxapi::ChannelEMDataHandle >( emDataNode );

        if( emDataIter != emList->end( ) )
        {
            std::cout << "    Received Power: " << emData->getReceivedPower( )->getValue( ) << std::endl;
            std::cout << "    Mean Time of Arrival: " << emData->getMeanTimeOfArrival( )->getValue( ) << std::endl;
            std::cout << "    Delay Spread: " << emData->getDelaySpread( )->getValue( ) << std::endl;
            std::cout << "    Paths: " << std::endl;

            remcom::rxapi::GetPathsJobHandle paths = remcom::rxapi::GetPathsJob::New( );
            remcom::rxapi::GetPathsModeHandle mode = remcom::rxapi::GetPathsMode::New( );

            mode->setValue( remcom::rxapi::GetPathsMode::GivenChannel );

            paths->setPathResultsDatabase( database );
            paths->setMode( mode );
            paths->setID( channelData->getChannelID( )->getValue( ) );

            if( !paths->isValid( ) )
            {
                return;
            }

            paths->execute( );

            remcom::rxapi::PathData::Iterator iter = paths->getResult( )->firstChild( "Path" );
            while( iter != paths->getResult( )->end( ) )
            {
                // There may not be any EM data on this path id.  Remember, paths are found and
                // saved in the db, but they may get eliminated.  We must check so we don't try
                // to reference pointers to data that doesn't exist (and crash).
                remcom::rxapi::NodeHandle pathNode = iter.getNode( );
                remcom::rxapi::PathDataHandle path = remcom::rxapi::Handle_cast< remcom::rxapi::PathDataHandle >( pathNode );
                remcom::rxapi::Handle< remcom::rxapi::PathFullEMData > h_allEm = path->getEMDataList()->getEMData();
                if ( h_allEm != NULL )
                {
                    std::cout << "        [" << path->getPathID( )->getValue( ) << "]" << std::endl;
                    std::cout << "            Received Power: " << h_allEm->getReceivedPower( )->getValue( ) << std::endl;
                    std::cout << "            Time of Arrival: " << h_allEm->getTimeOfArrival( )->getValue( ) << std::endl;
                }

                ++iter;
            }

            std::cout << std::endl;
        }

        ++channelDataIter;
    }
}


double 
writeChannelToConsole( remcom::rxapi::PathResultsDatabaseHandle database, int txSet, int txPoint, int rxSet, int rxPoint )
{
    remcom::rxapi::GetChannelJobHandle channel = remcom::rxapi::GetChannelJob::New( );

    channel->setPathResultsDatabase( database );
    channel->setTxSetID( txSet );
    channel->setTxPointID( txPoint );
    channel->setRxSetID( rxSet );
    channel->setRxPointID( rxPoint );

    if( !channel->isValid( ) )
    {
        return 0;
    }

    channel->execute( );

    if( channel->getResult( )->getChannel( ) == NULL )
    {
        std::cout << "Tx Set: " << txSet << std::endl;
        std::cout << "Tx Point: " << txPoint << std::endl;
        std::cout << "Rx Set: " << rxSet << std::endl;
        std::cout << "Rx Point: " << rxPoint << std::endl;

        std::cout << "    No paths" << std::endl;
        std::cout << std::endl;
        return 0;
    }

    remcom::rxapi::GetPathsJobHandle paths = remcom::rxapi::GetPathsJob::New( );
    remcom::rxapi::GetPathsModeHandle mode = remcom::rxapi::GetPathsMode::New( );

    mode->setValue( remcom::rxapi::GetPathsMode::GivenChannel );

    paths->setPathResultsDatabase( database );
    paths->setMode( mode );
    paths->setID( channel->getResult( )->getChannel( )->getChannelID( ) );

    if( !paths->isValid( ) )
    {
        return 0;
    }

    paths->execute( );

    std::cout << "Tx Set: " << txSet << std::endl;
    std::cout << "Tx Point: " << txPoint << std::endl;
    std::cout << "Rx Set: " << rxSet << std::endl;
    std::cout << "Rx Point: " << rxPoint << std::endl;
    
    double rcvPower = 0;

    if( channel->getResult( ) != NULL )
    {
        rcvPower = channel->getResult( )->getChannel( )->getChannelEMList( )->getChannelEMData( )->getReceivedPower( )->getValue( );
        std::cout << "    Received Power: " << rcvPower << std::endl;
        std::cout << "    Mean Time of Arrival: " << channel->getResult( )->getChannel( )->getChannelEMList( )->getChannelEMData( )->getMeanTimeOfArrival( )->getValue( ) << std::endl;
        std::cout << "    Delay Spread: " << channel->getResult( )->getChannel( )->getChannelEMList( )->getChannelEMData( )->getDelaySpread( )->getValue( ) << std::endl;
        std::cout << "    Paths: " << std::endl;

        remcom::rxapi::PathData::Iterator iter = paths->getResult( )->firstChild( "Path" );
        while( iter != paths->getResult( )->end( ) )
        {
            // There may not be any EM data on this path id.  Remember, paths are found and
            // saved in the db, but they may get eliminated.  We must check so we don't try
            // to reference pointers to data that doesn't exist (and crash).
            remcom::rxapi::NodeHandle pathNode = iter.getNode( );
            remcom::rxapi::PathDataHandle path = remcom::rxapi::Handle_cast< remcom::rxapi::PathDataHandle >( pathNode );
            remcom::rxapi::Handle< remcom::rxapi::PathFullEMData > h_allEm = path->getEMDataList()->getEMData();
            if ( h_allEm != NULL )
            {
                std::cout << "        [" << path->getPathID( )->getValue( ) << "]" << std::endl;
                std::cout << "            Received Power: " << h_allEm->getReceivedPower( )->getValue( ) << std::endl;
                std::cout << "            Time of Arrival: " << h_allEm->getTimeOfArrival( )->getValue( ) << std::endl;
            }

            ++iter;
        }

        std::cout << std::endl;
    }

    return rcvPower;
}
