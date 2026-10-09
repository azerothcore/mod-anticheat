--
DELETE FROM `module_string` WHERE `module` = 'anticheat' AND `id` IN (1,2,3,4,5,6);
INSERT INTO `module_string` (`module`, `id`, `string`) VALUES
('anticheat', 1, '|cffffff00[|cffff0000ANTICHEAT ALERT|r|cffffff00]:|r |cFFFF8C00|r |cFFFF8C00[|Hplayer:{}|h{}|h|r|cFFFF8C00] - Latency: {} ms - Report: {}'),
('anticheat', 2, '|cffffff00[|cffff0000ANTICHEAT ALERT|r|cffffff00]:|r POSSIBLE TELEPORT HACK DETECTED|cFFFF8C00 [|Hplayer:{}|h{}|h|r|cFFFF8C00]|r - Latency: {} ms - GPS Diff x: {}, y: {}, z: {}'),
('anticheat', 3, '|cffffff00[|cffff0000ANTICHEAT ALERT|r|cffffff00]:|r POSSIBLE IGNORE CONTROL HACK DETECTED|cFFFF8C00 {}|r - Latency: {} ms'),
('anticheat', 4, '|cffffff00[|cffff0000ANTICHEAT ALERT|r|cffffff00]:|r TELEPORT HACK USED WHILE DUELING|cFFFF8C00 {}|r - Latency: {} ms vs |cFFFF8C00 {}|r - Latency: {} ms.'),
('anticheat', 5, '|cffffff00[|cffff0000ANTICHEAT ALERT|r|cffffff00]:|r BG Start Teleport\\Exploit Hack DETECTED|cFFFF8C00[|Hplayer:{}|h{}|h|r|cFFFF8C00] - Latency: {} ms'),
('anticheat', 6, '|cffffff00[|cffff0000COUNTER MEASURE ALERT|r|cffffff00]:|r |cFFFF8C00|r {} |cFFFF8C00[|Hplayer:{}|h{}|h|r|cFFFF8C00]');

DELETE FROM `module_string_locale` WHERE `module` = 'anticheat' AND `id` IN (1,2,3,4,5,6);
INSERT INTO `module_string_locale` (`module`, `id`, `locale`, `string`) VALUES
-- ID 1: generic alert
('anticheat', 1, 'koKR', '|cffffff00[|cffff0000부정행위 경고|r|cffffff00]:|r |cFFFF8C00|r |cFFFF8C00[|Hplayer:{}|h{}|h|r|cFFFF8C00] - 지연 시간: {} ms - 보고: {}'),
('anticheat', 1, 'frFR', '|cffffff00[|cffff0000ALERTE ANTICHEAT|r|cffffff00]:|r |cFFFF8C00|r |cFFFF8C00[|Hplayer:{}|h{}|h|r|cFFFF8C00] - Latence : {} ms - Rapport : {}'),
('anticheat', 1, 'deDE', '|cffffff00[|cffff0000ANTICHEAT-WARNUNG|r|cffffff00]:|r |cFFFF8C00|r |cFFFF8C00[|Hplayer:{}|h{}|h|r|cFFFF8C00] - Latenz: {} ms - Bericht: {}'),
('anticheat', 1, 'zhCN', '|cffffff00[|cffff0000反作弊警报|r|cffffff00]:|r |cFFFF8C00|r |cFFFF8C00[|Hplayer:{}|h{}|h|r|cFFFF8C00] - 延迟: {} ms - 报告: {}'),
('anticheat', 1, 'zhTW', '|cffffff00[|cffff0000反作弊警報|r|cffffff00]:|r |cFFFF8C00|r |cFFFF8C00[|Hplayer:{}|h{}|h|r|cFFFF8C00] - 延遲: {} ms - 報告: {}'),
('anticheat', 1, 'esES', '|cffffff00[|cffff0000ALERTA ANTITRAMPAS|r|cffffff00]:|r |cFFFF8C00|r |cFFFF8C00[|Hplayer:{}|h{}|h|r|cFFFF8C00] - Latencia: {} ms - Reporte: {}'),
('anticheat', 1, 'esMX', '|cffffff00[|cffff0000ALERTA ANTITRAMPAS|r|cffffff00]:|r |cFFFF8C00|r |cFFFF8C00[|Hplayer:{}|h{}|h|r|cFFFF8C00] - Latencia: {} ms - Reporte: {}'),
('anticheat', 1, 'ruRU', '|cffffff00[|cffff0000ПРЕДУПРЕЖДЕНИЕ АНТИЧИТА|r|cffffff00]:|r |cFFFF8C00|r |cFFFF8C00[|Hplayer:{}|h{}|h|r|cFFFF8C00] - Задержка: {} мс - Отчёт: {}'),
-- ID 2: teleport hack
('anticheat', 2, 'koKR', '|cffffff00[|cffff0000부정행위 경고|r|cffffff00]:|r 가능한 텔레포트 핵 감지됨|cFFFF8C00 [|Hplayer:{}|h{}|h|r|cFFFF8C00]|r - 지연 시간: {} ms - GPS 차이 x: {}, y: {}, z: {}'),
('anticheat', 2, 'frFR', '|cffffff00[|cffff0000ALERTE ANTICHEAT|r|cffffff00]:|r POSSIBLE TÉLÉPORT HACK DÉTECTÉ|cFFFF8C00 [|Hplayer:{}|h{}|h|r|cFFFF8C00]|r - Latence : {} ms - Diff GPS x: {}, y: {}, z: {}'),
('anticheat', 2, 'deDE', '|cffffff00[|cffff0000ANTICHEAT-WARNUNG|r|cffffff00]:|r MÖGLICHER TELEPORT-HACK ERKANNT|cFFFF8C00 [|Hplayer:{}|h{}|h|r|cFFFF8C00]|r - Latenz: {} ms - GPS-Abweichung x: {}, y: {}, z: {}'),
('anticheat', 2, 'zhCN', '|cffffff00[|cffff0000反作弊警报|r|cffffff00]:|r 检测到可能的传送外挂|cFFFF8C00 [|Hplayer:{}|h{}|h|r|cFFFF8C00]|r - 延迟: {} ms - GPS 差值 x: {}, y: {}, z: {}'),
('anticheat', 2, 'zhTW', '|cffffff00[|cffff0000反作弊警報|r|cffffff00]:|r 偵測到可能的傳送外掛|cFFFF8C00 [|Hplayer:{}|h{}|h|r|cFFFF8C00]|r - 延遲: {} ms - GPS 差值 x: {}, y: {}, z: {}'),
('anticheat', 2, 'esES', '|cffffff00[|cffff0000ALERTA ANTITRAMPAS|r|cffffff00]:|r POSIBLE HACK DE TELEPORTE DETECTADO|cFFFF8C00 [|Hplayer:{}|h{}|h|r|cFFFF8C00]|r - Latencia: {} ms - GPS Diff x: {}, y: {}, z: {}'),
('anticheat', 2, 'esMX', '|cffffff00[|cffff0000ALERTA ANTITRAMPAS|r|cffffff00]:|r POSIBLE HACK DE TELEPORTE DETECTADO|cFFFF8C00 [|Hplayer:{}|h{}|h|r|cFFFF8C00]|r - Latencia: {} ms - GPS Diff x: {}, y: {}, z: {}'),
('anticheat', 2, 'ruRU', '|cffffff00[|cffff0000ПРЕДУПРЕЖДЕНИЕ АНТИЧИТА|r|cffffff00]:|r ОБНАРУЖЕН ВОЗМОЖНЫЙ ЧИТ-ТЕЛЕПОРТ|cFFFF8C00 [|Hplayer:{}|h{}|h|r|cFFFF8C00]|r - Задержка: {} мс - Разница GPS x: {}, y: {}, z: {}'),
-- ID 3: ignore control hack
('anticheat', 3, 'koKR', '|cffffff00[|cffff0000부정행위 경고|r|cffffff00]:|r 가능한 제어 무시 핵 감지됨|cFFFF8C00 {}|r - 지연 시간: {} ms'),
('anticheat', 3, 'frFR', '|cffffff00[|cffff0000ALERTE ANTICHEAT|r|cffffff00]:|r POSSIBLE IGNORE CONTROL HACK DÉTECTÉ|cFFFF8C00 {}|r - Latence : {} ms'),
('anticheat', 3, 'deDE', '|cffffff00[|cffff0000ANTICHEAT-WARNUNG|r|cffffff00]:|r MÖGLICHER IGNORE-CONTROL-HACK ERKANNT|cFFFF8C00 {}|r - Latenz: {} ms'),
('anticheat', 3, 'zhCN', '|cffffff00[|cffff0000反作弊警报|r|cffffff00]:|r 检测到可能的忽略控制外挂|cFFFF8C00 {}|r - 延迟: {} ms'),
('anticheat', 3, 'zhTW', '|cffffff00[|cffff0000反作弊警報|r|cffffff00]:|r 偵測到可能的忽略控制外掛|cFFFF8C00 {}|r - 延遲: {} ms'),
('anticheat', 3, 'esES', '|cffffff00[|cffff0000ALERTA ANTITRAMPAS|r|cffffff00]:|r POSIBLE CONTROL DE HACK DETECTADO IGNORARADO|cFFFF8C00 {}|r - Latencia: {} ms'),
('anticheat', 3, 'esMX', '|cffffff00[|cffff0000ALERTA ANTITRAMPAS|r|cffffff00]:|r POSIBLE CONTROL DE HACK DETECTADO IGNORARADO|cFFFF8C00 {}|r - Latencia: {} ms'),
('anticheat', 3, 'ruRU', '|cffffff00[|cffff0000ПРЕДУПРЕЖДЕНИЕ АНТИЧИТА|r|cffffff00]:|r ОБНАРУЖЕН ВОЗМОЖНЫЙ ЧИТ-ОБХОД КОНТРОЛЯ|cFFFF8C00 {}|r - Задержка: {} мс'),
-- ID 4: teleport while dueling
('anticheat', 4, 'koKR', '|cffffff00[|cffff0000부정행위 경고|r|cffffff00]:|r 결투 중 텔레포트 핵 사용|cFFFF8C00 {}|r - 지연 시간: {} ms vs |cFFFF8C00 {}|r - 지연 시간: {} ms.'),
('anticheat', 4, 'frFR', '|cffffff00[|cffff0000ALERTE ANTICHEAT|r|cffffff00]:|r TÉLÉPORT HACK UTILISÉ PENDANT UN DUEL|cFFFF8C00 {}|r - Latence : {} ms vs |cFFFF8C00 {}|r - Latence : {} ms.'),
('anticheat', 4, 'deDE', '|cffffff00[|cffff0000ANTICHEAT-WARNUNG|r|cffffff00]:|r TELEPORT-HACK WÄHREND EINES DUELS VERWENDET|cFFFF8C00 {}|r - Latenz: {} ms vs |cFFFF8C00 {}|r - Latenz: {} ms.'),
('anticheat', 4, 'zhCN', '|cffffff00[|cffff0000反作弊警报|r|cffffff00]:|r 决斗中使用传送外挂|cFFFF8C00 {}|r - 延迟: {} ms vs |cFFFF8C00 {}|r - 延迟: {} ms。'),
('anticheat', 4, 'zhTW', '|cffffff00[|cffff0000反作弊警報|r|cffffff00]:|r 決鬥中使用傳送外掛|cFFFF8C00 {}|r - 延遲: {} ms vs |cFFFF8C00 {}|r - 延遲: {} ms。'),
('anticheat', 4, 'esES', '|cffffff00[|cffff0000ALERTA ANTITRAMPAS|r|cffffff00]:|r HACK DE TELEPORTE USADO DURANTE UN DUELO|cFFFF8C00 {}|r - Latencia: {} ms vs |cFFFF8C00 {}|r - Latencia: {} ms.'),
('anticheat', 4, 'esMX', '|cffffff00[|cffff0000ALERTA ANTITRAMPAS|r|cffffff00]:|r HACK DE TELEPORTE USADO DURANTE UN DUELO|cFFFF8C00 {}|r - Latencia: {} ms vs |cFFFF8C00 {}|r - Latencia: {} ms.'),
('anticheat', 4, 'ruRU', '|cffffff00[|cffff0000ПРЕДУПРЕЖДЕНИЕ АНТИЧИТА|r|cffffff00]:|r ЧИТ-ТЕЛЕПОРТ ИСПОЛЬЗОВАН ВО ВРЕМЯ ДУЭЛИ|cFFFF8C00 {}|r - Задержка: {} мс vs |cFFFF8C00 {}|r - Задержка: {} мс.'),
-- ID 5: BG start teleport/exploit
('anticheat', 5, 'koKR', '|cffffff00[|cffff0000부정행위 경고|r|cffffff00]:|r 전장 시작 텔레포트\\익스플로잇 핵 감지됨|cFFFF8C00[|Hplayer:{}|h{}|h|r|cFFFF8C00] - 지연 시간: {} ms'),
('anticheat', 5, 'frFR', '|cffffff00[|cffff0000ALERTE ANTICHEAT|r|cffffff00]:|r Téléport Début BG\\Exploit Hack DÉTECTÉ|cFFFF8C00[|Hplayer:{}|h{}|h|r|cFFFF8C00] - Latence : {} ms'),
('anticheat', 5, 'deDE', '|cffffff00[|cffff0000ANTICHEAT-WARNUNG|r|cffffff00]:|r BG-START-TELEPORT\\EXPLOIT-HACK ERKANNT|cFFFF8C00[|Hplayer:{}|h{}|h|r|cFFFF8C00] - Latenz: {} ms'),
('anticheat', 5, 'zhCN', '|cffffff00[|cffff0000反作弊警报|r|cffffff00]:|r 检测到战场开始传送\\漏洞外挂|cFFFF8C00[|Hplayer:{}|h{}|h|r|cFFFF8C00] - 延迟: {} ms'),
('anticheat', 5, 'zhTW', '|cffffff00[|cffff0000反作弊警報|r|cffffff00]:|r 偵測到戰場開始傳送\\漏洞外掛|cFFFF8C00[|Hplayer:{}|h{}|h|r|cFFFF8C00] - 延遲: {} ms'),
('anticheat', 5, 'esES', '|cffffff00[|cffff0000ALERTA ANTITRAMPAS|r|cffffff00]:|r HACK DE TELEPORTE\\EXPLOIT AL INICIO DE BG DETECTADO|cFFFF8C00[|Hplayer:{}|h{}|h|r|cFFFF8C00] - Latencia: {} ms'),
('anticheat', 5, 'esMX', '|cffffff00[|cffff0000ALERTA ANTITRAMPAS|r|cffffff00]:|r HACK DE TELEPORTE\\EXPLOIT AL INICIO DE BG DETECTADO|cFFFF8C00[|Hplayer:{}|h{}|h|r|cFFFF8C00] - Latencia: {} ms'),
('anticheat', 5, 'ruRU', '|cffffff00[|cffff0000ПРЕДУПРЕЖДЕНИЕ АНТИЧИТА|r|cffffff00]:|r ОБНАРУЖЕН ЧИТ-ТЕЛЕПОРТ\\ЭКСПЛОЙТ НА СТАРТЕ БГ|cFFFF8C00[|Hplayer:{}|h{}|h|r|cFFFF8C00] - Задержка: {} мс'),
-- ID 6: counter measure
('anticheat', 6, 'koKR', '|cffffff00[|cffff0000대응 조치 경고|r|cffffff00]:|r |cFFFF8C00|r {} |cFFFF8C00[|Hplayer:{}|h{}|h|r|cFFFF8C00]'),
('anticheat', 6, 'frFR', '|cffffff00[|cffff0000ALERTE CONTRE MESURE|r|cffffff00]:|r |cFFFF8C00|r {} |cFFFF8C00[|Hplayer:{}|h{}|h|r|cFFFF8C00]'),
('anticheat', 6, 'deDE', '|cffffff00[|cffff0000GEGENMASSNAHMEN-WARNUNG|r|cffffff00]:|r |cFFFF8C00|r {} |cFFFF8C00[|Hplayer:{}|h{}|h|r|cFFFF8C00]'),
('anticheat', 6, 'zhCN', '|cffffff00[|cffff0000反制措施警报|r|cffffff00]:|r |cFFFF8C00|r {} |cFFFF8C00[|Hplayer:{}|h{}|h|r|cFFFF8C00]'),
('anticheat', 6, 'zhTW', '|cffffff00[|cffff0000反制措施警報|r|cffffff00]:|r |cFFFF8C00|r {} |cFFFF8C00[|Hplayer:{}|h{}|h|r|cFFFF8C00]'),
('anticheat', 6, 'esES', '|cffffff00[|cffff0000ALERTA CONTRA MEDIDAS|r|cffffff00]:|r |cFFFF8C00|r {} |cFFFF8C00[|Hplayer:{}|h{}|h|r|cFFFF8C00]'),
('anticheat', 6, 'esMX', '|cffffff00[|cffff0000ALERTA CONTRA MEDIDAS|r|cffffff00]:|r |cFFFF8C00|r {} |cFFFF8C00[|Hplayer:{}|h{}|h|r|cFFFF8C00]'),
('anticheat', 6, 'ruRU', '|cffffff00[|cffff0000ПРЕДУПРЕЖДЕНИЕ О КОНТРМЕРАХ|r|cffffff00]:|r |cFFFF8C00|r {} |cFFFF8C00[|Hplayer:{}|h{}|h|r|cFFFF8C00]');
