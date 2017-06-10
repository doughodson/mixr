
# ---------------------------------------------------------------------
# Setup environment variables for framework and examples
# (Edit as required)
# ---------------------------------------------------------------------

# ---
# MXPR root
# ---
MXRP_ROOT=$PWD
export MXRP_ROOT

# ---
# MXRP 3rd party (e.g. cigi, JSBSim) root
# ---
MXRP_3RD_PARTY_ROOT=$PWD/../mxrp_3rdparty
export MXRP_3RD_PARTY_ROOT

# ---
# Update the loader library path with our 3rd party library path
# ---
LD_LIBRARY_PATH=$MXRP_3RD_PARTY_ROOT/lib:${LD_LIBRARY_PATH}
export LD_LIBRARY_PATH

# ---
# Update path to search for binaries in our 3rd party library path
# ---
PATH=$MXRP_3RD_PARTY_ROOT/bin:$PATH
export PATH

