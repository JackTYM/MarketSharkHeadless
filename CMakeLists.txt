cmake_minimum_required(VERSION 3.10)

project(mclib)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS_DEBUG "-g")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

# Build the library as a static library instead of a shared library
add_library(mclib STATIC
	mclib/src/mclib/block/Banner.cpp
	mclib/src/mclib/block/Beacon.cpp
	mclib/src/mclib/block/Bed.cpp
	mclib/src/mclib/block/Block.cpp
	mclib/src/mclib/block/BlockEntity.cpp
	mclib/src/mclib/block/BrewingStand.cpp
	mclib/src/mclib/block/Chest.cpp
	mclib/src/mclib/block/Dispenser.cpp
	mclib/src/mclib/block/Dropper.cpp
	mclib/src/mclib/block/EnchantmentTable.cpp
	mclib/src/mclib/block/EndGateway.cpp
	mclib/src/mclib/block/FlowerPot.cpp
	mclib/src/mclib/block/Furnace.cpp
	mclib/src/mclib/block/Hopper.cpp
	mclib/src/mclib/block/InventoryBlock.cpp
	mclib/src/mclib/block/Jukebox.cpp
	mclib/src/mclib/block/MonsterSpawner.cpp
	mclib/src/mclib/block/NoteBlock.cpp
	mclib/src/mclib/block/Piston.cpp
	mclib/src/mclib/block/RedstoneComparator.cpp
	mclib/src/mclib/block/ShulkerBox.cpp
	mclib/src/mclib/block/Sign.cpp
	mclib/src/mclib/block/Skull.cpp
	mclib/src/mclib/common/DataBuffer.cpp
	mclib/src/mclib/common/DyeColor.cpp
	mclib/src/mclib/common/MCString.cpp
	mclib/src/mclib/common/Position.cpp
	mclib/src/mclib/common/UUID.cpp
	mclib/src/mclib/common/VarInt.cpp
	mclib/src/mclib/core/AuthToken.cpp
	mclib/src/mclib/core/Client.cpp
	mclib/src/mclib/core/ClientSettings.cpp
	mclib/src/mclib/core/Compression.cpp
	mclib/src/mclib/core/Connection.cpp
	mclib/src/mclib/core/Encryption.cpp
	mclib/src/mclib/core/PlayerManager.cpp
	mclib/src/mclib/entity/EntityManager.cpp
	mclib/src/mclib/entity/Metadata.cpp
	mclib/src/mclib/inventory/Hotbar.cpp
	mclib/src/mclib/inventory/Inventory.cpp
	mclib/src/mclib/inventory/Slot.cpp
	mclib/src/mclib/nbt/NBT.cpp
	mclib/src/mclib/nbt/Tag.cpp
	mclib/src/mclib/network/IPAddress.cpp
	mclib/src/mclib/network/Network.cpp
	mclib/src/mclib/network/Socket.cpp
	mclib/src/mclib/network/TCPSocket.cpp
	mclib/src/mclib/network/UDPSocket.cpp
	mclib/src/mclib/protocol/packets/Packet.cpp
	mclib/src/mclib/protocol/packets/PacketDispatcher.cpp
	mclib/src/mclib/protocol/packets/PacketFactory.cpp
	mclib/src/mclib/protocol/packets/PacketHandler.cpp
	mclib/src/mclib/protocol/Protocol.cpp
	mclib/src/mclib/util/Forge.cpp
	mclib/src/mclib/util/Hash.cpp
	mclib/src/mclib/util/HTTPClient.cpp
	mclib/src/mclib/util/Utility.cpp
	mclib/src/mclib/util/VersionFetcher.cpp
	mclib/src/mclib/util/Yggdrasil.cpp
	mclib/src/mclib/world/Chunk.cpp
	mclib/src/mclib/world/World.cpp
)

add_definitions(-DMCLIB_EXPORTS -DCURL_STATICLIB)

# Find required packages
find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(ZLIB REQUIRED)
find_package(Curses REQUIRED)

# Include directories
target_include_directories(mclib
		PUBLIC mclib/include
		client/util
		PRIVATE ${OPENSSL_INCLUDE_DIR} ${CURL_INCLUDE_DIRS} ${ZLIB_INCLUDE_DIRS} ${CURSES_INCLUDE_DIRS}
)

# Link libraries
if (WIN32)
	target_link_libraries(mclib ${ZLIB_LIBRARIES} ${CURL_LIBRARIES} ${OPENSSL_LIBRARIES} Wldap32.lib Crypt32.lib ${CURSES_LIBRARIES})
else ()
	target_link_libraries(mclib ${ZLIB_LIBRARIES} ${CURL_LIBRARIES} ${OPENSSL_LIBRARIES} ${CURSES_LIBRARIES})
endif ()

# Build the client executable
add_executable(client
		client/main.cpp
		client/util/Logger.cpp
		client/Server.cpp

		client/auth/Auth.cpp

		client/features/AutoOpen.h
		client/features/JoinSkyblock.h
        client/features/JoinSkyblock.cpp
		client/features/ChatLogger.h
        client/features/ChatLogger.cpp

		client/cofl/CoflNet.cpp
        client/cofl/CoflSessionManager.cpp
		client/features/AutoOpen.cpp
		client/features/QueueManager.h
		client/features/QueueManager.cpp
		client/features/AutoBuy.cpp
		client/features/AutoBuy.h
		client/util/FlipItem.cpp
		client/util/FlipItem.h
        client/features/Failsafes.cpp
        client/features/Failsafes.h
		client/util/Objects.cpp
		client/util/Objects.h
		client/util/Colors.h
		client/util/Colors.cpp
)

# Link the static mclib library with the client executable
target_link_libraries(client PRIVATE mclib ixwebsocket uuid)

# Install the client executable
install(TARGETS client
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)