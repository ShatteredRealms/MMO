#)####################################################################################
#   _____ _           _   _                    _   _____            _               #
#  / ____| |         | | | |                  | | |  __ \          | |              #
# | (___ | |__   __ _| |_| |_ ___ _ __ ___  __| | | |__) |___  __ _| |_ __ ___  ___ #
#  \___ \| '_ \ / _` | __| __/ _ \ '__/ _ \/ _` | |  _  // _ \/ _` | | '_ ` _ \/ __|#
#  ____) | | | | (_| | |_| ||  __/ | |  __/ (_| | | | \ \  __/ (_| | | | | | | \__ \#
# |_____/|_| |_|\__,_|\__|\__\___|_|  \___|\__,_| |_|  \_\___|\__,_|_|_| |_| |_|___/#
#####################################################################################

#
# Makefile for building and testing
#

# Gets the directory containing the Makefile
ROOT_DIR = $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

PROTO_DIR=$(ROOT_DIR)

PROTO_FILES = $(shell find "$(PROTO_DIR)/sro" -name '*.proto')


#   _____                    _
#  |_   _|                  | |
#    | | __ _ _ __ __ _  ___| |_ ___
#    | |/ _` | '__/ _` |/ _ \ __/ __|
#    | | (_| | | | (_| |  __/ |_\__ \
#    \_/\__,_|_|  \__, |\___|\__|___/
#                  __/ |
#                 |___/

.PHONY: clean-protos protos $(PROTO_FILES)

clean-protos:
	mkdir -p "$(ROOT_DIR)/pkg"
	rm -rf "$(ROOT_DIR)/pkg"

protos: clean-protos $(PROTO_FILES) move-protos

$(PROTO_FILES):
	protoc "$@" \
		-I "$(PROTO_DIR)" \
		--go_out="$(ROOT_DIR)" \
		--go-grpc_out="$(ROOT_DIR)" \
		--grpc-gateway_out="$(ROOT_DIR)" \
		--grpc-gateway_opt "logtostderr=true"

move-protos:
	mv -v "$(ROOT_DIR)/github.com/ShatteredRealms" "$(ROOT_DIR)/pkg/"
	rm -r "$(ROOT_DIR)/github.com"
