DELETE FROM acore_string WHERE `entry` BETWEEN 30087 AND 30096;
INSERT INTO acore_string (entry, content_default) VALUES
(30087, 'AntiCheat: SpeedHack Detected for %s, normal distance for this time and speed = %f, distance from packet = %f'),
(30088, 'AntiCheat: FlyHack Detected for %s , player can not fly'),
(30089, 'AntiCheat: FlyHack Detected for %s, player has Swimming flag, but not in water'),
(30090, 'AntiCheat: FlyHack Detected for %s'),
(30091, 'AntiCheat: DoubleJump Detected for %s'),
(30092, 'AntiCheat: Fake Jumper Detected for %s'),
(30093, 'AntiCheat: FakeFlying mode Detected for %s'),
(30094, 'AntiCheat: Wallclimb Detected for Account id : %u, Player %s, diffZ = %f, distance = %f, angle = %f, Map = %s, mapId = %u, X = %f, Y = %f, Z = %f'),
(30095, 'AntiCheat: Ignore control Hack Detected for Account : %u, Player : %s'),
(30096, 'AntiCheat: NoFallingDamage Hack Detected for Account : %u, Player : %s');
