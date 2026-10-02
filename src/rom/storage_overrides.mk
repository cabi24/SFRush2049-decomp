# Generated from rom_owned_data.json storage_blocks; do not hand edit.
ifeq ($(COMPILER),ido)
LDFLAGS := -T rom_owned_storage.ld $(LDFLAGS)
$(ELF): rom_owned_storage.ld rom_owned_data.json
O_FILES := $(filter-out $(BUILD_DIR)/asm/us/7630.o,$(O_FILES))
$(BUILD_DIR)/src/rom/lib_7630.o: CFLAGS := $(filter-out -O%,$(CFLAGS)) -O2
$(BUILD_DIR)/src/rom/lib_7630.o: src/rom/lib_7630.c rom_owned_data.json include/PR/os_thread.h
	@mkdir -p $(dir $@)
	@echo "CC $< (complete owned storage)"
	$(V)$(CC) -c -g0 $(CFLAGS) -o $@ $<
	$(V)$(OBJCOPY) --rename-section .bss=.bss.vi_manager $@
$(BUILD_DIR)/src/rom/lib_cc50.o: src/rom/lib_cc50.c rom_owned_data.json include/PR/os.h include/PR/os_ai.h include/PR/os_cont.h include/PR/os_message.h include/PR/os_pfs.h include/PR/os_pi.h include/PR/os_thread.h include/PR/os_time.h include/game/attract.h include/game/audio.h include/game/battle.h include/game/boost.h include/game/camera.h include/game/car.h include/game/carsel.h include/game/checkpoint.h include/game/collision.h include/game/crash.h include/game/drivetrain.h include/game/drone.h include/game/effects.h include/game/game.h include/game/garage.h include/game/ghost.h include/game/gstate.h include/game/hiscore.h include/game/hud.h include/game/init.h include/game/input.h include/game/math.h include/game/maxpath.h include/game/menu.h include/game/minimap.h include/game/model.h include/game/multiplayer.h include/game/music.h include/game/network.h include/game/particles.h include/game/pdu.h include/game/physics.h include/game/position.h include/game/race.h include/game/record.h include/game/render.h include/game/replay.h include/game/resurrect.h include/game/road.h include/game/save.h include/game/select.h include/game/shadow.h include/game/sound.h include/game/state.h include/game/structs.h include/game/stunt.h include/game/targets.h include/game/timer.h include/game/tire.h include/game/tournament.h include/game/track.h include/game/vecmath.h include/game/visuals.h include/game/weapon.h include/game/weather.h include/game/wings.h include/game/world.h include/game_types.h include/include_asm.h include/inflate/inflate.h include/labels.inc include/m2c_types.h include/macro.inc include/macros.h include/rom_auto.h include/types.h src/rom/rom_tu.h tools/conveyor/jobs/scoring.py cloud/work/integration_B24/context_hashes.json tools/conveyor/pipeline/owned_existing_storage.py
	@mkdir -p $(dir $@)
	@echo "CC $< (existing complete owned storage)"
	$(V)$(PYTHON) -m tools.conveyor.pipeline.owned_existing_storage assert timer_services
	$(V)$(CC) -c -g0 -O1 -mips2 -G 0 -non_shared -Xcpluscomm -Wab,-r4300_mul $(INCLUDE_CFLAGS) -o $@ $<
	$(V)$(OBJCOPY) --rename-section .bss=.bss.timer_services $@
endif
