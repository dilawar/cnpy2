import numpy as np
a = np.load( "./a.npy" )
print( "Shapes %s" % str(a.shape) )
print( "dtype %s " %  a.dtype )
print( "Got ", a )
print a['time']
print a["value"]
print a['time'].shape, a['value'].shape
