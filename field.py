#!/usr/bin/python
from PIL import Image, ImageColor, ImageDraw
import itertools
from math import sqrt

def dist(pos1, pos2):
    return sqrt(pow(pos1[0] - pos2[0], 2) + pow(pos1[1] - pos2[1], 2))

# lazy and slow whatever :^)
def closest(i,j):
    min_pix = None
    min_dist = 1000
    for x,y in itertools.product(xrange(w), xrange(h)):
        if mesh[x][y] == 0: continue
        if dist((x,y), (i,j)) < min_dist:
            min_dist = dist((x,y), (i,j))
            min_pix = (x,y)
    return min_pix

im = Image.open('in2.png')
pix = im.load()

w,h = im.size
mesh = []
out = []
for i in range(w):
    mesh.append([])
    out.append([])
    for j in range(h):
        if (pix[i,j][0] > 200 and pix[i,j][1] > 200 and pix[i,j][2] > 200):
            mesh[i].append(1) 
        else:
            mesh[i].append(0)
        out[i].append([0,0])

for x,y in itertools.product(xrange(w), xrange(h)):
    print (x,y)
    if mesh[x][y] == 0:
        close = closest(x,y)
        out_xy = (close[0]-x, close[1]-y)
        out[x][y][0] = out_xy[0] #/ dist((0,0), out_xy)
        out[x][y][1] = out_xy[1] #/ dist((0,0), out_xy)
    else:
        out[x][y] = [0,0]

out_im = Image.new('RGBA', (w*10,h*10))
draw = ImageDraw.Draw(out_im)
'''out_im = Image.new('RGBA', (w,h))
for x in xrange(0,w):
    for y in xrange(0,h):
        color = ImageColor.getrgb('white') if mesh[x][y] == 1 else ImageColor.getrgb('black')
        draw.point([x,y],fill=color)'''

for x in xrange(0,w):
    for y in xrange(0,h):
        if (mesh[x][y] == 0):
            v = out[x][y]
            v_x = v[0] * 2 / dist((0,0), v)
            v_y = v[1] * 2 / dist((0,0), v)
            p1 = (x*10+5 + v_x, y*10+5 + v_y)
            p2 = (x*10+5 - v_x, y*10+5 - v_y)
            draw.line([p2, p1], fill=128)
draw.ellipse([0, 0, 5, 5])
out_im.show()

with open('mesh', 'w') as f:
    f.write('abc123')
    for x in xrange(0,w):
        for y in xrange(0,h):
                f.write(' ')
                f.write(str(out[x][y][0]))
                f.write(' ')
                f.write(str(out[x][y][1]))
