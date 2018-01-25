import numpy as np
import sqlite3
import argparse
import matplotlib.pyplot as plt
import matplotlib.cm as cm


def wattsToDB(watts):
    return 10. * np.log10(watts) + 30.

def DBtoWatts(db):
    return 10.**((db - 30.)/10.)

def retreiveAllData(db_fn='/Users/mcrouse/Research/RandomizedMAC/raytracing/results/rosslyn_streets.sql'):
    conn = sqlite3.connect(db_fn)
    c = conn.cursor()
    # c.execute("SELECT * FROM channel_utd \
    #                         JOIN channel \
    #                         JOIN tx \
    #                             WHERE channel_utd.channel_id = channel.channel_id \
    #                             AND channel.tx_id = tx.tx_id;")

    c.execute("SELECT * FROM channel_utd \
                            JOIN channel \
                            JOIN rx \
                                WHERE channel_utd.channel_id = channel.channel_id \
                                AND channel.rx_id = rx.rx_id;")



    return c.fetchall()
    #r = [wattsToDB(v[3]) for v in c.fetchall()]

def fetchTx(tx_id, db_fn='/Users/mcrouse/Research/RandomizedMAC/raytracing/results/rosslyn_streets.sql'):
    conn = sqlite3.connect(db_fn)
    c = conn.cursor()
    c.execute("SELECT * FROM channel_utd \
                            JOIN channel \
                            JOIN tx \
                                WHERE channel_utd.channel_id = channel.channel_id \
                                AND channel.tx_id = tx.tx_id \
                                AND channel.tx_id = {};".format(tx_id))
                           
    row = c.fetchone()
    x = row[-3]
    y = row[-2]

    c.execute("SELECT * FROM channel_utd \
                            JOIN channel \
                            JOIN tx \
                                WHERE channel_utd.channel_id = channel.channel_id \
                                AND channel.tx_id = tx.tx_id \
                                AND tx.x = {} \
                                AND tx.y = {};".format(x, y))
                           

    r = [wattsToDB(v[3]) for v in c.fetchall()]

    return {'x' : x, 'y' : y, 'power': np.array(r)}

def plotAllTx(numTx, db_fn='/Users/mcrouse/Research/RandomizedMAC/raytracing/results/rosslyn_streets.sql'):
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='polar')
    rs = []
    for i in range(numTx):
        r = fetchTx(i, db_fn)
        ax.plot(np.linspace(0, 2*np.pi, r['power'].shape[0]), r['power'])
        rs.append(r['power'])

    s_max = np.max([np.max(rs[i]) for i in range(numTx)])
    s_min = np.min([np.min(rs[i]) for i in range(numTx)])
    print s_max, s_min
    ax.set_rmax(s_max)
    ax.set_rmin(s_min)
    ax.set_rticks(np.arange(-150,-50,25))
    ax.grid(True)


def plotTx(db_fn='../results/output.sql', num_racks_per_row=8, num_racks=4, fig=None, ax=None):

    r = retreiveAllData(db_fn=db_fn)
    print r

    tx_id = int(db_fn.split("_")[2]) + 1
    target_rx_id = int(db_fn.split("_")[3].split(".")[0]) + 1
    # print tx_id, target_rx_id
    print len(r)

    if fig is None:
        fig = plt.figure()
        ax = fig.add_subplot(111)

    xs = []
    ys = []
    rcv_dbs = []
    legend = []
    for row in r:
        print row[1]
        xs.append(row[-3])
        ys.append(row[-2])
        db = wattsToDB(row[3])
        print row[1], xs[-1], ys[-1], db
        rcv_dbs.append(wattsToDB(row[3]))
        m = 's'
        # cax =ax.scatter(row[-3], row[-2], c=db, marker='s', cmap=cm.jet)
        if row[1] == target_rx_id:
            target_x = row[-3]
            target_y = row[-2]
            # print "here: ", db
            ax.plot(row[-3], row[-2], c='r', markersize=15, marker='o', zorder=1, mfc='none', linestyle = 'None')
            legend.append('Target Rx')
        if row[1] == tx_id:
            target_x = row[-3]
            target_y = row[-2]
            print 'here'
            ax.plot(row[-3], row[-2], c='b', markersize=15, marker='o', zorder=1, mfc='none', linestyle = 'None')
            legend.append('Tx')

    
    cax = ax.scatter(xs, ys, c=rcv_dbs, cmap=cm.jet, marker='s', zorder=2, vmin=-250, vmax=0)
    ax.grid(True)
    cbar = fig.colorbar(cax)
    cbar.set_label("Received Power (dBm)")
    plt.xlabel('position (m)')
    plt.ylabel('position (m)')
    plt.title("Tx: %d - Target Rx: %d" % (tx_id, target_rx_id))
    plt.legend(legend)
    plt.xlim([0,5])
    plt.ylim([-6,0])
    # plt.axis('equal')
    plt.show()
 