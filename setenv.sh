
# ---------------------------------------------------------------------
# Setup environment variables for framework and examples
# (Edit as required)
# ---------------------------------------------------------------------

# ---
# MIXR root
# ---
MIXR_ROOT=$PWD
export MIXR_ROOT

# ---
# MIXR 3rd party (e.g. cigi, JSBSim) root
# ---
MIXR_3RD_PARTY_ROOT=$PWD/../mixr_3rdparty
export MIXR_3RD_PARTY_ROOT

# ---
# Update the loader library path with our 3rd party library path
# ---
LD_LIBRARY_PATH=$MIXR_3RD_PARTY_ROOT/lib:${LD_LIBRARY_PATH}
export LD_LIBRARY_PATH

# ---
# Update path to search for binaries in our 3rd party library path
# ---
export PATH=$MIXR_3RD_PARTY_ROOT/bin:$PATH
