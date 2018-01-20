#include <rxapi/Node.h>
#include <rxapi/ProgressReporter.h>

#include <rxapischema/Output.h>
#include <rxapischema/PathResultsDatabase.h>
#include <rxapischema/TxPointToRxSetGroup.h>

void writeAllChannelsToConsole( remcom::rxapi::PathResultsDatabaseHandle database, int txSet, int rxSet );
double writeChannelToConsole( remcom::rxapi::PathResultsDatabaseHandle database, int txSet, int txPoint, int rxSet, int rxPoint );
