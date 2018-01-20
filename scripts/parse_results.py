import numpy as np
import sqlite3
import argparse
import matplotlib.pyplot as plt

a = 5.

def wattsToDB(watts):
    return 10. * np.log10(watts) + 30.

def DBtoWatts(db):
    return 10.**((db - 30.)/10.)

def retreiveAllData(db_fn='/Users/mcrouse/Research/RandomizedMAC/raytracing/results/rosslyn_streets.sql'):
    conn = sqlite3.connect(db_fn)
    c = conn.cursor()
    c.execute("SELECT * FROM channel_utd \
                            JOIN channel \
                            JOIN tx \
                                WHERE channel_utd.channel_id = channel.channel_id \
                                AND channel.tx_id = tx.tx_id;")


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


def plotTxRow(startidx=0, endidx=36, offset=7):
    leg = []
    fig = plt.figure()
    ax = fig.add_subplot(111, projection='polar')
    for i in range(startidx,endidx,offset):
        r = fetchTx(i, db_fn='/Users/mcrouse/Research/RandomizedMAC/raytracing/results/rosslyn_grid6x6.sql')
        plotTx(r, fig, ax)
        leg.append("[{}, {}]".format(r['x'], r['y']))
    ax.legend(leg, loc='center left', bbox_to_anchor=(1, 0.5))

def plotTx(r, fig=None, ax=None):
    if fig is None:
        fig = plt.figure()
        ax = fig.add_subplot(111, projection='polar')
    ax.plot(np.linspace(0, 2*np.pi, r['power'].shape[0]), r['power'])
    ax.set_rmax(-20)
    ax.set_rmin(np.min(r['power']))
    ax.set_rticks(np.arange(-150,0,25))
    ax.grid(True)

 