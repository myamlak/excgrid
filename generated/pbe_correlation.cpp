// excgrid codegen source: the Perdew-Burke-Ernzerhof 1996 GGA correlation
// functional.  Fresh authorship; formula from the PBE paper
// (docs/mainpage.md references).  Regenerate with tools/regenerate.py.

// Auto-generated file, do not modify
#include "excgrid/kernel.hpp"

// The excgrid GGA kernel skeleton (fresh authorship; BSD-3-Clause).
// Emits one excgrid::XcKernelValue function per GGA functional: the energy
// density, the spin-density derivatives, and the three sigma derivatives.
// It also emits the functional's SECOND-DERIVATIVE tier: one function filling
// the contract's materialised matrix, from the same expression as the kernel.
// The matrix is the contract's upper triangle, row-major over the five
// components the kernel takes, in identifier order - the GGA tier's whole
// span.  Which of them a caller's matrix is over is the caller's mask, and the
// registry re-packs the two; the kinetic-energy-density rows and columns are
// not emitted at all, because a GGA energy density does not read tau.
// No spin-edge branches: the definitions' Tiny guards keep every expression
// finite at the spin and gamma edges (the LDA skeleton's exact-limit
// machinery is not needed for the gradient-corrected forms, whose edge
// contributions vanish with the spin density).
// The banner and the kernel.hpp include are emitted by the calling .ey, which
// always precedes this skeleton, so they are not repeated here.

#include <cmath>
#include <limits>

namespace excgrid {

XcKernelValue PbeCorrelation(
    double rhoA, double rhoB, double sigmaAa, double sigmaAb, double sigmaBb) {
    constexpr double eps = std::numeric_limits<double>::epsilon();
    [[maybe_unused]] constexpr double Pi = 3.14159265358979323846;
    // The sigma-edge guard the generator adds to a vanishing gradient radical
    // (ExGuardSigmaRadical in excgrid_generate.ys carries the measurement).
    // Its value has to be written HERE rather than injected from the
    // definitions: the printer renders a number below 1e-16 as "0.", so a
    // constant small enough to be absorbed by every normal sigma cannot reach
    // the expression through yacas at all.  1e-300 is absorbed by every sigma
    // above ~1e-284 and leaves the sum at exactly sigma, which is what keeps
    // the guarded arithmetic bit-identical to the unguarded one.
    [[maybe_unused]] constexpr double SigmaGuard = 1e-300;

    XcKernelValue result;

    if (rhoA + rhoB < 2. * eps)
    {
        return result;
    }

    const double C1180 = 2. * sigmaAb;
    const double C1181 = rhoA + rhoB;
    const double C1182 = rhoA - rhoB;
    const double C1183 = 1. / 3.;
    const double C1184 = 2. / 3.;
    const double C1185 = 4. / 3.;
    const double C1186 = std::pow(Pi, 2);
    const double C1187 = 0.9999999999999999 * C1182;
    const double C1188 = C1186 * C1181;
    const double C1189 = Pi * C1181;
    const double C1190 = sigmaAa + C1180;
    const double C1191 = C1182 / C1181;
    const double C1192 = std::pow(2., C1183);
    const double C1193 = 3. * C1188;
    const double C1194 = 4. * C1189;
    const double C1195 = C1190 + sigmaBb;
    const double C1196 = C1192 - 1.;
    const double C1197 = C1187 / C1181;
    const double C1198 = std::pow(C1191, 4);
    const double C1199 = 2. * C1196;
    const double C1200 = 3.4198418683227306 * C1196;
    const double C1201 = C1197 + 1.;
    const double C1202 = 1. - C1197;
    const double C1203 = 1. - C1198;
    const double C1204 = 3. / C1194;
    const double C1205 = std::sqrt(C1195);
    const double C1206 = std::pow(C1193, C1183);
    const double C1207 = C1206 / Pi;
    const double C1208 = std::sqrt(C1204);
    const double C1209 = std::pow(C1201, C1184);
    const double C1210 = std::pow(C1201, C1185);
    const double C1211 = std::pow(C1202, C1184);
    const double C1212 = std::pow(C1202, C1185);
    const double C1213 = std::pow(C1204, C1183);
    const double C1214 = std::pow(C1204, C1184);
    const double C1215 = 0.11125 * C1213;
    const double C1216 = 0.20548 * C1213;
    const double C1217 = 0.2137 * C1213;
    const double C1218 = 0.49294 * C1214;
    const double C1219 = 0.49671 * C1214;
    const double C1220 = 0.62517 * C1214;
    const double C1221 = 0.88026 * C1208;
    const double C1222 = 1.6382 * C1208;
    const double C1223 = 3.3662 * C1208;
    const double C1224 = 3.5876 * C1213;
    const double C1225 = 3.6231 * C1213;
    const double C1226 = 6.1977 * C1213;
    const double C1227 = C1209 + C1211;
    const double C1228 = C1210 + C1212;
    const double C1229 = std::sqrt(C1207);
    const double C1230 = std::sqrt(C1213);
    const double C1231 = 7.5957 * C1230;
    const double C1232 = 10.357 * C1230;
    const double C1233 = 14.1189 * C1230;
    const double C1234 = C1229 * C1227;
    const double C1235 = C1215 + 1.;
    const double C1236 = C1216 + 1.;
    const double C1237 = C1217 + 1.;
    const double C1238 = C1228 - 2.;
    const double C1239 = C1227 / 2.;
    const double C1240 = 2. * C1234;
    const double C1241 = C1231 + C1224;
    const double C1242 = C1232 + C1225;
    const double C1243 = C1233 + C1226;
    const double C1244 = std::pow(C1239, 3);
    const double C1245 = 0.3109069086965489e-1 * C1244;
    const double C1246 = C1181 * C1240;
    const double C1247 = C1241 + C1222;
    const double C1248 = C1242 + C1221;
    const double C1249 = C1243 + C1223;
    const double C1250 = C1247 + C1218;
    const double C1251 = C1248 + C1219;
    const double C1252 = C1249 + C1220;
    const double C1253 = C1246 / 2.;
    const double C1254 = 0.0310907 * C1252;
    const double C1255 = 0.0337738 * C1251;
    const double C1256 = 0.0621814 * C1250;
    const double C1257 = C1253 + 1e-16;
    const double C1258 = 2. * C1257;
    const double C1259 = 1. / C1254;
    const double C1260 = 1. / C1255;
    const double C1261 = 1. / C1256;
    const double C1262 = C1259 + 1.;
    const double C1263 = C1260 + 1.;
    const double C1264 = C1261 + 1.;
    const double C1265 = C1205 / C1258;
    const double C1266 = std::log(C1262);
    const double C1267 = std::log(C1263);
    const double C1268 = std::log(C1264);
    const double C1269 = std::pow(C1265, 2);
    const double C1270 = 0.06672455060314922 * C1269;
    const double C1271 = C1235 * C1267;
    const double C1272 = C1236 * C1266;
    const double C1273 = C1237 * C1268;
    const double C1274 = 0.0310907 * C1272;
    const double C1275 = 0.0621814 * C1273;
    const double C1276 = C1271 * C1238;
    const double C1277 = C1276 * C1203;
    const double C1278 = C1275 - C1274;
    const double C1279 = 0.0337738 * C1277;
    const double C1280 = C1278 * C1238;
    const double C1281 = C1198 * C1280;
    const double C1282 = C1279 / C1200;
    const double C1283 = C1282 - C1275;
    const double C1284 = C1281 / C1199;
    const double C1285 = C1283 + C1284;
    const double C1286 = C1285 / C1245;
    const double C1287 = -C1286;
    const double C1288 = std::exp(C1287);
    const double C1289 = C1288 - 1.;
    const double C1290 = 0.3109069086965489e-1 * C1289;
    const double C1291 = C1270 / C1290;

    result.exc =
        C1181 *
        (C1285 + 0.3109069086965489e-1 * C1244 *
                     std::log(0.06672455060314922 * C1269 * (C1291 + 1.) /
                                  (0.3109069086965489e-1 * ((C1291 + std::pow(C1291, 2)) + 1.)) +
                              1.));

    const double C1293 = 1;
    const double C1294 = 1;
    const double C1295 = 0.9999999999999999 * C1294 + 0.;
    const double C1296 = C1186 * C1293;
    const double C1297 = Pi * C1293;
    const double C1298 = C1294 / C1181 - C1293 * C1182 / std::pow(C1181, 2);
    const double C1299 = 3. * C1296;
    const double C1300 = 4. * C1297;
    const double C1301 = C1295 / C1181 - C1293 * C1187 / std::pow(C1181, 2);
    const double C1302 = 4 * std::pow(C1191, 3) * C1298;
    const double C1303 = 0.;
    const double C1304 = C1301;
    const double C1305 = -C1301;
    const double C1306 = -C1302;
    const double C1307 = -3. * C1300 / std::pow(C1194, 2);
    const double C1308 = C1183 * std::pow(C1193, C1183 - 1) * C1299;
    const double C1309 = C1308 / Pi;
    const double C1310 = C1184 * std::pow(C1201, C1184 - 1) * C1304;
    const double C1311 = C1185 * std::pow(C1201, C1185 - 1) * C1304;
    const double C1312 = C1184 * std::pow(C1202, C1184 - 1) * C1305;
    const double C1313 = C1185 * std::pow(C1202, C1185 - 1) * C1305;
    const double C1314 = C1183 * std::pow(C1204, C1183 - 1) * C1307;
    const double C1315 = C1184 * std::pow(C1204, C1184 - 1) * C1307;
    const double C1316 = 0.11125 * C1314;
    const double C1317 = 0.20548 * C1314;
    const double C1318 = 0.2137 * C1314;
    const double C1319 = 0.49294 * C1315;
    const double C1320 = 0.49671 * C1315;
    const double C1321 = 0.62517 * C1315;
    const double C1322 = 0.88026 * C1307 / (2 * std::sqrt(C1204));
    const double C1323 = 1.6382 * C1307 / (2 * std::sqrt(C1204));
    const double C1324 = 3.3662 * C1307 / (2 * std::sqrt(C1204));
    const double C1325 = 3.5876 * C1314;
    const double C1326 = 3.6231 * C1314;
    const double C1327 = 6.1977 * C1314;
    const double C1328 = C1310 + C1312;
    const double C1329 = C1311 + C1313;
    const double C1330 = 7.5957 * C1314 / (2 * std::sqrt(C1213));
    const double C1331 = 10.357 * C1314 / (2 * std::sqrt(C1213));
    const double C1332 = 0.141189e2 * C1314 / (2 * std::sqrt(C1213));
    const double C1333 = C1309 * C1227 / (2 * std::sqrt(C1207)) + std::sqrt(C1207) * C1328;
    const double C1334 = C1316;
    const double C1335 = C1317;
    const double C1336 = C1318;
    const double C1337 = C1329;
    const double C1338 = C1328 / 2.;
    const double C1339 = 2. * C1333;
    const double C1340 = C1330 + C1325;
    const double C1341 = C1331 + C1326;
    const double C1342 = C1332 + C1327;
    const double C1343 = 3 * std::pow(C1239, 2) * C1338;
    const double C1344 = 0.3109069086965489e-1 * C1343 + 0.;
    const double C1345 = C1240 * C1293 + C1181 * C1339;
    const double C1346 = C1340 + C1323;
    const double C1347 = C1341 + C1322;
    const double C1348 = C1342 + C1324;
    const double C1349 = C1346 + C1319;
    const double C1350 = C1347 + C1320;
    const double C1351 = C1348 + C1321;
    const double C1352 = C1345 / 2.;
    const double C1353 = 0.0310907 * C1351;
    const double C1354 = 0.0337738 * C1350;
    const double C1355 = 0.0621814 * C1349;
    const double C1356 = C1352;
    const double C1357 = 2. * C1356;
    const double C1358 = (-C1353) / std::pow(C1254, 2);
    const double C1359 = (-C1354) / std::pow(C1255, 2);
    const double C1360 = (-C1355) / std::pow(C1256, 2);
    const double C1361 = C1358;
    const double C1362 = C1359;
    const double C1363 = C1360;
    const double C1364 = (-C1357 * std::sqrt(C1195)) / std::pow(C1258, 2);
    const double C1365 = C1361 / C1262;
    const double C1366 = C1362 / C1263;
    const double C1367 = C1363 / C1264;
    const double C1368 = 2 * C1265 * C1364;
    const double C1369 = 0.0667245506031492 * C1368;
    const double C1370 = C1267 * C1334 + C1235 * C1366;
    const double C1371 = C1266 * C1335 + C1236 * C1365;
    const double C1372 = C1268 * C1336 + C1237 * C1367;
    const double C1373 = 0.0310907 * C1371;
    const double C1374 = 0.0621814 * C1372;
    const double C1375 = C1238 * C1370 + C1271 * C1337;
    const double C1376 = C1203 * C1375 + C1276 * C1306;
    const double C1377 = C1374 - C1373;
    const double C1378 = 0.0337738 * C1376;
    const double C1379 = C1238 * C1377 + C1278 * C1337;
    const double C1380 = C1280 * C1302 + C1198 * C1379;
    const double C1381 = C1378 / C1200 - C1303 * C1279 / std::pow(C1200, 2);
    const double C1382 = C1381 - C1374;
    const double C1383 = C1380 / C1199;
    const double C1384 = C1382 + C1383;
    const double C1385 = C1384 / C1245 - C1344 * C1285 / std::pow(C1245, 2);
    const double C1386 = -C1385;
    const double C1387 = std::exp(C1287) * C1386;
    const double C1388 = C1387;
    const double C1389 = 0.3109069086965489e-1 * C1388 + 0.;
    const double C1390 = C1369 / C1290 - C1389 * C1270 / std::pow(C1290, 2);

    result.vrhoA =
        ((((0. * C1181 +
            (0. * C1181 + (C1285 + 0.3109069086965489e-1 * C1244 *
                                       std::log(0.06672455060314922 * C1269 * (C1291 + 1.) /
                                                    (0.3109069086965489e-1 *
                                                     ((C1291 + std::pow(C1291, 2)) + 1.)) +
                                                1.))) *
                C1293) +
           1. * C1181 * C1384) +
          C1181 * 0.3109069086965489e-1 *
              std::log(0.06672455060314922 * C1269 * (C1291 + 1.) /
                           (0.3109069086965489e-1 * ((C1291 + std::pow(C1291, 2)) + 1.)) +
                       1.) *
              C1343) +
         C1368 * C1181 * 0.3109069086965489e-1 * C1244 * 0.06672455060314922 * (C1291 + 1.) /
             (0.3109069086965489e-1 * ((C1291 + std::pow(C1291, 2)) + 1.) *
              (0.06672455060314922 * C1269 * (C1291 + 1.) /
                   (0.3109069086965489e-1 * ((C1291 + std::pow(C1291, 2)) + 1.)) +
               1.))) +
        C1390 * C1181 * 0.3109069086965489e-1 * C1244 *
            (0.3109069086965489e-1 * ((C1291 + std::pow(C1291, 2)) + 1.) * 0.06672455060314922 *
                 C1269 -
             0.06672455060314922 * C1269 * (C1291 + 1.) * 0.3109069086965489e-1 * (2 * C1291 + 1)) /
            (std::pow(0.3109069086965489e-1 * ((C1291 + std::pow(C1291, 2)) + 1.), 2) *
             (0.06672455060314922 * C1269 * (C1291 + 1.) /
                  (0.3109069086965489e-1 * ((C1291 + std::pow(C1291, 2)) + 1.)) +
              1.));

    const double C1391 = 1;
    const double C1392 = -1;
    const double C1393 = 0.9999999999999999 * C1392 + 0.;
    const double C1394 = C1186 * C1391;
    const double C1395 = Pi * C1391;
    const double C1396 = C1392 / C1181 - C1391 * C1182 / std::pow(C1181, 2);
    const double C1397 = 3. * C1394;
    const double C1398 = 4. * C1395;
    const double C1399 = C1393 / C1181 - C1391 * C1187 / std::pow(C1181, 2);
    const double C1400 = 4 * std::pow(C1191, 3) * C1396;
    const double C1401 = 0.;
    const double C1402 = C1399;
    const double C1403 = -C1399;
    const double C1404 = -C1400;
    const double C1405 = -3. * C1398 / std::pow(C1194, 2);
    const double C1406 = C1183 * std::pow(C1193, C1183 - 1) * C1397;
    const double C1407 = C1406 / Pi;
    const double C1408 = C1184 * std::pow(C1201, C1184 - 1) * C1402;
    const double C1409 = C1185 * std::pow(C1201, C1185 - 1) * C1402;
    const double C1410 = C1184 * std::pow(C1202, C1184 - 1) * C1403;
    const double C1411 = C1185 * std::pow(C1202, C1185 - 1) * C1403;
    const double C1412 = C1183 * std::pow(C1204, C1183 - 1) * C1405;
    const double C1413 = C1184 * std::pow(C1204, C1184 - 1) * C1405;
    const double C1414 = 0.11125 * C1412;
    const double C1415 = 0.20548 * C1412;
    const double C1416 = 0.2137 * C1412;
    const double C1417 = 0.49294 * C1413;
    const double C1418 = 0.49671 * C1413;
    const double C1419 = 0.62517 * C1413;
    const double C1420 = 0.88026 * C1405 / (2 * std::sqrt(C1204));
    const double C1421 = 1.6382 * C1405 / (2 * std::sqrt(C1204));
    const double C1422 = 3.3662 * C1405 / (2 * std::sqrt(C1204));
    const double C1423 = 3.5876 * C1412;
    const double C1424 = 3.6231 * C1412;
    const double C1425 = 6.1977 * C1412;
    const double C1426 = C1408 + C1410;
    const double C1427 = C1409 + C1411;
    const double C1428 = 7.5957 * C1412 / (2 * std::sqrt(C1213));
    const double C1429 = 10.357 * C1412 / (2 * std::sqrt(C1213));
    const double C1430 = 0.141189e2 * C1412 / (2 * std::sqrt(C1213));
    const double C1431 = C1407 * C1227 / (2 * std::sqrt(C1207)) + std::sqrt(C1207) * C1426;
    const double C1432 = C1414;
    const double C1433 = C1415;
    const double C1434 = C1416;
    const double C1435 = C1427;
    const double C1436 = C1426 / 2.;
    const double C1437 = 2. * C1431;
    const double C1438 = C1428 + C1423;
    const double C1439 = C1429 + C1424;
    const double C1440 = C1430 + C1425;
    const double C1441 = 3 * std::pow(C1239, 2) * C1436;
    const double C1442 = 0.3109069086965489e-1 * C1441 + 0.;
    const double C1443 = C1240 * C1391 + C1181 * C1437;
    const double C1444 = C1438 + C1421;
    const double C1445 = C1439 + C1420;
    const double C1446 = C1440 + C1422;
    const double C1447 = C1444 + C1417;
    const double C1448 = C1445 + C1418;
    const double C1449 = C1446 + C1419;
    const double C1450 = C1443 / 2.;
    const double C1451 = 0.0310907 * C1449;
    const double C1452 = 0.0337738 * C1448;
    const double C1453 = 0.0621814 * C1447;
    const double C1454 = C1450;
    const double C1455 = 2. * C1454;
    const double C1456 = (-C1451) / std::pow(C1254, 2);
    const double C1457 = (-C1452) / std::pow(C1255, 2);
    const double C1458 = (-C1453) / std::pow(C1256, 2);
    const double C1459 = C1456;
    const double C1460 = C1457;
    const double C1461 = C1458;
    const double C1462 = (-C1455 * std::sqrt(C1195)) / std::pow(C1258, 2);
    const double C1463 = C1459 / C1262;
    const double C1464 = C1460 / C1263;
    const double C1465 = C1461 / C1264;
    const double C1466 = 2 * C1265 * C1462;
    const double C1467 = 0.0667245506031492 * C1466;
    const double C1468 = C1267 * C1432 + C1235 * C1464;
    const double C1469 = C1266 * C1433 + C1236 * C1463;
    const double C1470 = C1268 * C1434 + C1237 * C1465;
    const double C1471 = 0.0310907 * C1469;
    const double C1472 = 0.0621814 * C1470;
    const double C1473 = C1238 * C1468 + C1271 * C1435;
    const double C1474 = C1203 * C1473 + C1276 * C1404;
    const double C1475 = C1472 - C1471;
    const double C1476 = 0.0337738 * C1474;
    const double C1477 = C1238 * C1475 + C1278 * C1435;
    const double C1478 = C1280 * C1400 + C1198 * C1477;
    const double C1479 = C1476 / C1200 - C1401 * C1279 / std::pow(C1200, 2);
    const double C1480 = C1479 - C1472;
    const double C1481 = C1478 / C1199;
    const double C1482 = C1480 + C1481;
    const double C1483 = C1482 / C1245 - C1442 * C1285 / std::pow(C1245, 2);
    const double C1484 = -C1483;
    const double C1485 = std::exp(C1287) * C1484;
    const double C1486 = C1485;
    const double C1487 = 0.3109069086965489e-1 * C1486 + 0.;
    const double C1488 = C1467 / C1290 - C1487 * C1270 / std::pow(C1290, 2);

    result.vrhoB =
        ((((0. * C1181 +
            (0. * C1181 + (C1285 + 0.3109069086965489e-1 * C1244 *
                                       std::log(0.06672455060314922 * C1269 * (C1291 + 1.) /
                                                    (0.3109069086965489e-1 *
                                                     ((C1291 + std::pow(C1291, 2)) + 1.)) +
                                                1.))) *
                C1391) +
           1. * C1181 * C1482) +
          C1181 * 0.3109069086965489e-1 *
              std::log(0.06672455060314922 * C1269 * (C1291 + 1.) /
                           (0.3109069086965489e-1 * ((C1291 + std::pow(C1291, 2)) + 1.)) +
                       1.) *
              C1441) +
         C1466 * C1181 * 0.3109069086965489e-1 * C1244 * 0.06672455060314922 * (C1291 + 1.) /
             (0.3109069086965489e-1 * ((C1291 + std::pow(C1291, 2)) + 1.) *
              (0.06672455060314922 * C1269 * (C1291 + 1.) /
                   (0.3109069086965489e-1 * ((C1291 + std::pow(C1291, 2)) + 1.)) +
               1.))) +
        C1488 * C1181 * 0.3109069086965489e-1 * C1244 *
            (0.3109069086965489e-1 * ((C1291 + std::pow(C1291, 2)) + 1.) * 0.06672455060314922 *
                 C1269 -
             0.06672455060314922 * C1269 * (C1291 + 1.) * 0.3109069086965489e-1 * (2 * C1291 + 1)) /
            (std::pow(0.3109069086965489e-1 * ((C1291 + std::pow(C1291, 2)) + 1.), 2) *
             (0.06672455060314922 * C1269 * (C1291 + 1.) /
                  (0.3109069086965489e-1 * ((C1291 + std::pow(C1291, 2)) + 1.)) +
              1.));

    const double C1489 = 0.;
    const double C1490 = 1;
    const double C1491 = C1490;
    const double C1492 = C1489 / C1181;
    const double C1493 = 0.;
    const double C1494 = C1492;
    const double C1495 = -C1492;
    const double C1496 = C1184 * std::pow(C1201, C1184 - 1) * C1494;
    const double C1497 = C1185 * std::pow(C1201, C1185 - 1) * C1494;
    const double C1498 = C1184 * std::pow(C1202, C1184 - 1) * C1495;
    const double C1499 = C1185 * std::pow(C1202, C1185 - 1) * C1495;
    const double C1500 = C1496 + C1498;
    const double C1501 = C1497 + C1499;
    const double C1502 = std::sqrt(C1207) * C1500;
    const double C1503 = C1501;
    const double C1504 = C1500 / 2.;
    const double C1505 = 2. * C1502;
    const double C1506 = 3 * std::pow(C1239, 2) * C1504;
    const double C1507 = 0.3109069086965489e-1 * C1506 + 0.;
    const double C1508 = C1181 * C1505;
    const double C1509 = C1508 / 2.;
    const double C1510 = C1509;
    const double C1511 = 2. * C1510;
    const double C1512 =
        C1491 / (2 * std::sqrt(C1195) * C1258) - C1511 * std::sqrt(C1195) / std::pow(C1258, 2);
    const double C1513 = 2 * C1265 * C1512;
    const double C1514 = 0.0667245506031492 * C1513;
    const double C1515 = C1271 * C1503;
    const double C1516 = C1203 * C1515;
    const double C1517 = 0.0337738 * C1516;
    const double C1518 = C1278 * C1503;
    const double C1519 = C1198 * C1518;
    const double C1520 = C1517 / C1200 - C1493 * C1279 / std::pow(C1200, 2);
    const double C1521 = C1520;
    const double C1522 = C1519 / C1199;
    const double C1523 = C1521 + C1522;
    const double C1524 = C1523 / C1245 - C1507 * C1285 / std::pow(C1245, 2);
    const double C1525 = -C1524;
    const double C1526 = std::exp(C1287) * C1525;
    const double C1527 = C1526;
    const double C1528 = 0.3109069086965489e-1 * C1527 + 0.;
    const double C1529 = C1514 / C1290 - C1528 * C1270 / std::pow(C1290, 2);

    result.vsigmaAa =
        (((0. * C1181 + 1. * C1181 * C1523) +
          C1181 * 0.3109069086965489e-1 *
              std::log(0.06672455060314922 * C1269 * (C1291 + 1.) /
                           (0.3109069086965489e-1 * ((C1291 + std::pow(C1291, 2)) + 1.)) +
                       1.) *
              C1506) +
         C1513 * C1181 * 0.3109069086965489e-1 * C1244 * 0.06672455060314922 * (C1291 + 1.) /
             (0.3109069086965489e-1 * ((C1291 + std::pow(C1291, 2)) + 1.) *
              (0.06672455060314922 * C1269 * (C1291 + 1.) /
                   (0.3109069086965489e-1 * ((C1291 + std::pow(C1291, 2)) + 1.)) +
               1.))) +
        C1529 * C1181 * 0.3109069086965489e-1 * C1244 *
            (0.3109069086965489e-1 * ((C1291 + std::pow(C1291, 2)) + 1.) * 0.06672455060314922 *
                 C1269 -
             0.06672455060314922 * C1269 * (C1291 + 1.) * 0.3109069086965489e-1 * (2 * C1291 + 1)) /
            (std::pow(0.3109069086965489e-1 * ((C1291 + std::pow(C1291, 2)) + 1.), 2) *
             (0.06672455060314922 * C1269 * (C1291 + 1.) /
                  (0.3109069086965489e-1 * ((C1291 + std::pow(C1291, 2)) + 1.)) +
              1.));

    const double C1530 = 2.;
    const double C1531 = 0.;
    const double C1532 = C1530;
    const double C1533 = C1532;
    const double C1534 = C1531 / C1181;
    const double C1535 = 0.;
    const double C1536 = C1534;
    const double C1537 = -C1534;
    const double C1538 = C1184 * std::pow(C1201, C1184 - 1) * C1536;
    const double C1539 = C1185 * std::pow(C1201, C1185 - 1) * C1536;
    const double C1540 = C1184 * std::pow(C1202, C1184 - 1) * C1537;
    const double C1541 = C1185 * std::pow(C1202, C1185 - 1) * C1537;
    const double C1542 = C1538 + C1540;
    const double C1543 = C1539 + C1541;
    const double C1544 = std::sqrt(C1207) * C1542;
    const double C1545 = C1543;
    const double C1546 = C1542 / 2.;
    const double C1547 = 2. * C1544;
    const double C1548 = 3 * std::pow(C1239, 2) * C1546;
    const double C1549 = 0.3109069086965489e-1 * C1548 + 0.;
    const double C1550 = C1181 * C1547;
    const double C1551 = C1550 / 2.;
    const double C1552 = C1551;
    const double C1553 = 2. * C1552;
    const double C1554 =
        C1533 / (2 * std::sqrt(C1195) * C1258) - C1553 * std::sqrt(C1195) / std::pow(C1258, 2);
    const double C1555 = 2 * C1265 * C1554;
    const double C1556 = 0.0667245506031492 * C1555;
    const double C1557 = C1271 * C1545;
    const double C1558 = C1203 * C1557;
    const double C1559 = 0.0337738 * C1558;
    const double C1560 = C1278 * C1545;
    const double C1561 = C1198 * C1560;
    const double C1562 = C1559 / C1200 - C1535 * C1279 / std::pow(C1200, 2);
    const double C1563 = C1562;
    const double C1564 = C1561 / C1199;
    const double C1565 = C1563 + C1564;
    const double C1566 = C1565 / C1245 - C1549 * C1285 / std::pow(C1245, 2);
    const double C1567 = -C1566;
    const double C1568 = std::exp(C1287) * C1567;
    const double C1569 = C1568;
    const double C1570 = 0.3109069086965489e-1 * C1569 + 0.;
    const double C1571 = C1556 / C1290 - C1570 * C1270 / std::pow(C1290, 2);

    result.vsigmaAb =
        (((0. * C1181 + 1. * C1181 * C1565) +
          C1181 * 0.3109069086965489e-1 *
              std::log(0.06672455060314922 * C1269 * (C1291 + 1.) /
                           (0.3109069086965489e-1 * ((C1291 + std::pow(C1291, 2)) + 1.)) +
                       1.) *
              C1548) +
         C1555 * C1181 * 0.3109069086965489e-1 * C1244 * 0.06672455060314922 * (C1291 + 1.) /
             (0.3109069086965489e-1 * ((C1291 + std::pow(C1291, 2)) + 1.) *
              (0.06672455060314922 * C1269 * (C1291 + 1.) /
                   (0.3109069086965489e-1 * ((C1291 + std::pow(C1291, 2)) + 1.)) +
               1.))) +
        C1571 * C1181 * 0.3109069086965489e-1 * C1244 *
            (0.3109069086965489e-1 * ((C1291 + std::pow(C1291, 2)) + 1.) * 0.06672455060314922 *
                 C1269 -
             0.06672455060314922 * C1269 * (C1291 + 1.) * 0.3109069086965489e-1 * (2 * C1291 + 1)) /
            (std::pow(0.3109069086965489e-1 * ((C1291 + std::pow(C1291, 2)) + 1.), 2) *
             (0.06672455060314922 * C1269 * (C1291 + 1.) /
                  (0.3109069086965489e-1 * ((C1291 + std::pow(C1291, 2)) + 1.)) +
              1.));

    const double C1572 = 0.;
    const double C1573 = 1;
    const double C1574 = C1572 / C1181;
    const double C1575 = 0.;
    const double C1576 = C1574;
    const double C1577 = -C1574;
    const double C1578 = C1184 * std::pow(C1201, C1184 - 1) * C1576;
    const double C1579 = C1185 * std::pow(C1201, C1185 - 1) * C1576;
    const double C1580 = C1184 * std::pow(C1202, C1184 - 1) * C1577;
    const double C1581 = C1185 * std::pow(C1202, C1185 - 1) * C1577;
    const double C1582 = C1578 + C1580;
    const double C1583 = C1579 + C1581;
    const double C1584 = std::sqrt(C1207) * C1582;
    const double C1585 = C1583;
    const double C1586 = C1582 / 2.;
    const double C1587 = 2. * C1584;
    const double C1588 = 3 * std::pow(C1239, 2) * C1586;
    const double C1589 = 0.3109069086965489e-1 * C1588 + 0.;
    const double C1590 = C1181 * C1587;
    const double C1591 = C1590 / 2.;
    const double C1592 = C1591;
    const double C1593 = 2. * C1592;
    const double C1594 =
        C1573 / (2 * std::sqrt(C1195) * C1258) - C1593 * std::sqrt(C1195) / std::pow(C1258, 2);
    const double C1595 = 2 * C1265 * C1594;
    const double C1596 = 0.0667245506031492 * C1595;
    const double C1597 = C1271 * C1585;
    const double C1598 = C1203 * C1597;
    const double C1599 = 0.0337738 * C1598;
    const double C1600 = C1278 * C1585;
    const double C1601 = C1198 * C1600;
    const double C1602 = C1599 / C1200 - C1575 * C1279 / std::pow(C1200, 2);
    const double C1603 = C1602;
    const double C1604 = C1601 / C1199;
    const double C1605 = C1603 + C1604;
    const double C1606 = C1605 / C1245 - C1589 * C1285 / std::pow(C1245, 2);
    const double C1607 = -C1606;
    const double C1608 = std::exp(C1287) * C1607;
    const double C1609 = C1608;
    const double C1610 = 0.3109069086965489e-1 * C1609 + 0.;
    const double C1611 = C1596 / C1290 - C1610 * C1270 / std::pow(C1290, 2);

    result.vsigmaBb =
        (((0. * C1181 + 1. * C1181 * C1605) +
          C1181 * 0.3109069086965489e-1 *
              std::log(0.06672455060314922 * C1269 * (C1291 + 1.) /
                           (0.3109069086965489e-1 * ((C1291 + std::pow(C1291, 2)) + 1.)) +
                       1.) *
              C1588) +
         C1595 * C1181 * 0.3109069086965489e-1 * C1244 * 0.06672455060314922 * (C1291 + 1.) /
             (0.3109069086965489e-1 * ((C1291 + std::pow(C1291, 2)) + 1.) *
              (0.06672455060314922 * C1269 * (C1291 + 1.) /
                   (0.3109069086965489e-1 * ((C1291 + std::pow(C1291, 2)) + 1.)) +
               1.))) +
        C1611 * C1181 * 0.3109069086965489e-1 * C1244 *
            (0.3109069086965489e-1 * ((C1291 + std::pow(C1291, 2)) + 1.) * 0.06672455060314922 *
                 C1269 -
             0.06672455060314922 * C1269 * (C1291 + 1.) * 0.3109069086965489e-1 * (2 * C1291 + 1)) /
            (std::pow(0.3109069086965489e-1 * ((C1291 + std::pow(C1291, 2)) + 1.), 2) *
             (0.06672455060314922 * C1269 * (C1291 + 1.) /
                  (0.3109069086965489e-1 * ((C1291 + std::pow(C1291, 2)) + 1.)) +
              1.));

    return result;
}

void PbeCorrelationSecondDerivatives(double rhoA,
                                     double rhoB,
                                     double sigmaAa,
                                     double sigmaAb,
                                     double sigmaBb,
                                     PointSecondDerivativeMatrix& matrix) {
    [[maybe_unused]] constexpr double Pi = 3.14159265358979323846;
    [[maybe_unused]] constexpr double SigmaGuard = 1e-300;

    if (rhoA + rhoB < 2. * std::numeric_limits<double>::epsilon())
    {
        return;
    }

    const double C46 = 2. * sigmaAb;
    const double C47 = rhoA + rhoB;
    const double C48 = rhoA - rhoB;
    const double C49 = 1. / 3.;
    const double C50 = 2. / 3.;
    const double C51 = 4. / 3.;
    const double C52 = std::pow(Pi, 2);
    const double C53 = 0.9999999999999999 * C48;
    const double C54 = C52 * C47;
    const double C55 = Pi * C47;
    const double C56 = sigmaAa + C46;
    const double C57 = C48 / C47;
    const double C58 = std::pow(2., C49);
    const double C59 = 3. * C54;
    const double C60 = 4. * C55;
    const double C61 = C56 + sigmaBb;
    const double C62 = C58 - 1.;
    const double C63 = C53 / C47;
    const double C64 = std::pow(C57, 4);
    const double C65 = 2. * C62;
    const double C66 = 3.4198418683227306 * C62;
    const double C67 = C63 + 1.;
    const double C68 = 1. - C63;
    const double C69 = 1. - C64;
    const double C70 = 3. / C60;
    const double C71 = std::sqrt(C61);
    const double C72 = std::pow(C59, C49);
    const double C73 = C72 / Pi;
    const double C74 = std::sqrt(C70);
    const double C75 = std::pow(C67, C50);
    const double C76 = std::pow(C67, C51);
    const double C77 = std::pow(C68, C50);
    const double C78 = std::pow(C68, C51);
    const double C79 = std::pow(C70, C49);
    const double C80 = std::pow(C70, C50);
    const double C81 = 0.11125 * C79;
    const double C82 = 0.20548 * C79;
    const double C83 = 0.2137 * C79;
    const double C84 = 0.49294 * C80;
    const double C85 = 0.49671 * C80;
    const double C86 = 0.62517 * C80;
    const double C87 = 0.88026 * C74;
    const double C88 = 1.6382 * C74;
    const double C89 = 3.3662 * C74;
    const double C90 = 3.5876 * C79;
    const double C91 = 3.6231 * C79;
    const double C92 = 6.1977 * C79;
    const double C93 = C75 + C77;
    const double C94 = C76 + C78;
    const double C95 = std::sqrt(C73);
    const double C96 = std::sqrt(C79);
    const double C97 = 7.5957 * C96;
    const double C98 = 10.357 * C96;
    const double C99 = 14.1189 * C96;
    const double C100 = C95 * C93;
    const double C101 = C81 + 1.;
    const double C102 = C82 + 1.;
    const double C103 = C83 + 1.;
    const double C104 = C94 - 2.;
    const double C105 = C93 / 2.;
    const double C106 = 2. * C100;
    const double C107 = C97 + C90;
    const double C108 = C98 + C91;
    const double C109 = C99 + C92;
    const double C110 = std::pow(C105, 3);
    const double C111 = 0.3109069086965489e-1 * C110;
    const double C112 = C47 * C106;
    const double C113 = C107 + C88;
    const double C114 = C108 + C87;
    const double C115 = C109 + C89;
    const double C116 = C113 + C84;
    const double C117 = C114 + C85;
    const double C118 = C115 + C86;
    const double C119 = C112 / 2.;
    const double C120 = 0.0310907 * C118;
    const double C121 = 0.0337738 * C117;
    const double C122 = 0.0621814 * C116;
    const double C123 = C119 + 1e-16;
    const double C124 = 2. * C123;
    const double C125 = 1. / C120;
    const double C126 = 1. / C121;
    const double C127 = 1. / C122;
    const double C128 = C125 + 1.;
    const double C129 = C126 + 1.;
    const double C130 = C127 + 1.;
    const double C131 = C71 / C124;
    const double C132 = std::log(C128);
    const double C133 = std::log(C129);
    const double C134 = std::log(C130);
    const double C135 = std::pow(C131, 2);
    const double C136 = 0.06672455060314922 * C135;
    const double C137 = C101 * C133;
    const double C138 = C102 * C132;
    const double C139 = C103 * C134;
    const double C140 = 0.0310907 * C138;
    const double C141 = 0.0621814 * C139;
    const double C142 = C137 * C104;
    const double C143 = C142 * C69;
    const double C144 = C141 - C140;
    const double C145 = 0.0337738 * C143;
    const double C146 = C144 * C104;
    const double C147 = C64 * C146;
    const double C148 = C145 / C66;
    const double C149 = C148 - C141;
    const double C150 = C147 / C65;
    const double C151 = C149 + C150;
    const double C152 = C151 / C111;
    const double C153 = -C152;
    const double C154 = std::exp(C153);
    const double C155 = C154 - 1.;
    const double C156 = 0.3109069086965489e-1 * C155;
    const double C157 = C136 / C156;
    const double C162 = 1;
    const double C163 = 1;
    const double C164 = 0.9999999999999999 * C163 + 0.;
    const double C165 = C52 * C162;
    const double C166 = Pi * C162;
    const double C167 = C163 / C47 - C162 * C48 / std::pow(C47, 2);
    const double C168 = 3. * C165;
    const double C169 = 4. * C166;
    const double C170 = C164 / C47 - C162 * C53 / std::pow(C47, 2);
    const double C171 = 4 * std::pow(C57, 3) * C167;
    const double C172 = 0.;
    const double C173 = C170;
    const double C174 = -C170;
    const double C175 = -C171;
    const double C176 = -3. * C169 / std::pow(C60, 2);
    const double C177 = C49 * std::pow(C59, C49 - 1) * C168;
    const double C178 = C177 / Pi;
    const double C179 = C50 * std::pow(C67, C50 - 1) * C173;
    const double C180 = C51 * std::pow(C67, C51 - 1) * C173;
    const double C181 = C50 * std::pow(C68, C50 - 1) * C174;
    const double C182 = C51 * std::pow(C68, C51 - 1) * C174;
    const double C183 = C49 * std::pow(C70, C49 - 1) * C176;
    const double C184 = C50 * std::pow(C70, C50 - 1) * C176;
    const double C185 = 0.11125 * C183;
    const double C186 = 0.20548 * C183;
    const double C187 = 0.2137 * C183;
    const double C188 = 0.49294 * C184;
    const double C189 = 0.49671 * C184;
    const double C190 = 0.62517 * C184;
    const double C191 = 0.88026 * C176 / (2 * std::sqrt(C70));
    const double C192 = 1.6382 * C176 / (2 * std::sqrt(C70));
    const double C193 = 3.3662 * C176 / (2 * std::sqrt(C70));
    const double C194 = 3.5876 * C183;
    const double C195 = 3.6231 * C183;
    const double C196 = 6.1977 * C183;
    const double C197 = C179 + C181;
    const double C198 = C180 + C182;
    const double C199 = 7.5957 * C183 / (2 * std::sqrt(C79));
    const double C200 = 10.357 * C183 / (2 * std::sqrt(C79));
    const double C201 = 0.141189e2 * C183 / (2 * std::sqrt(C79));
    const double C202 = C178 * C93 / (2 * std::sqrt(C73)) + std::sqrt(C73) * C197;
    const double C203 = C185;
    const double C204 = C186;
    const double C205 = C187;
    const double C206 = C198;
    const double C207 = C197 / 2.;
    const double C208 = 2. * C202;
    const double C209 = C199 + C194;
    const double C210 = C200 + C195;
    const double C211 = C201 + C196;
    const double C212 = 3 * std::pow(C105, 2) * C207;
    const double C213 = 0.3109069086965489e-1 * C212 + 0.;
    const double C214 = C106 * C162 + C47 * C208;
    const double C215 = C209 + C192;
    const double C216 = C210 + C191;
    const double C217 = C211 + C193;
    const double C218 = C215 + C188;
    const double C219 = C216 + C189;
    const double C220 = C217 + C190;
    const double C221 = C214 / 2.;
    const double C222 = 0.0310907 * C220;
    const double C223 = 0.0337738 * C219;
    const double C224 = 0.0621814 * C218;
    const double C225 = C221;
    const double C226 = 2. * C225;
    const double C227 = (-C222) / std::pow(C120, 2);
    const double C228 = (-C223) / std::pow(C121, 2);
    const double C229 = (-C224) / std::pow(C122, 2);
    const double C230 = C227;
    const double C231 = C228;
    const double C232 = C229;
    const double C233 = (-C226 * std::sqrt(C61)) / std::pow(C124, 2);
    const double C234 = C230 / C128;
    const double C235 = C231 / C129;
    const double C236 = C232 / C130;
    const double C237 = 2 * C131 * C233;
    const double C238 = 0.0667245506031492 * C237;
    const double C239 = C133 * C203 + C101 * C235;
    const double C240 = C132 * C204 + C102 * C234;
    const double C241 = C134 * C205 + C103 * C236;
    const double C242 = 0.0310907 * C240;
    const double C243 = 0.0621814 * C241;
    const double C244 = C104 * C239 + C137 * C206;
    const double C245 = C69 * C244 + C142 * C175;
    const double C246 = C243 - C242;
    const double C247 = 0.0337738 * C245;
    const double C248 = C104 * C246 + C144 * C206;
    const double C249 = C146 * C171 + C64 * C248;
    const double C250 = C247 / C66 - C172 * C145 / std::pow(C66, 2);
    const double C251 = C250 - C243;
    const double C252 = C249 / C65;
    const double C253 = C251 + C252;
    const double C254 = C253 / C111 - C213 * C151 / std::pow(C111, 2);
    const double C255 = -C254;
    const double C256 = std::exp(C153) * C255;
    const double C257 = C256;
    const double C258 = 0.3109069086965489e-1 * C257 + 0.;
    const double C259 = C238 / C156 - C258 * C136 / std::pow(C156, 2);
    const double C481 = 0.;
    const double C482 =
        ((-C163) / std::pow(C47, 2) - (-C162 * C48 * 2 * C47) / std::pow(C47, 4)) * C162 -
        C163 * C162 / std::pow(C47, 2);
    const double C483 =
        (C481 / C47 +
         ((-C164) / std::pow(C47, 2) - (-C162 * C53 * 2 * C47) / std::pow(C47, 4)) * C162) -
        C164 * C162 / std::pow(C47, 2);
    const double C484 = 12 * std::pow(C57, 2) * std::pow(C167, 2) + 4 * std::pow(C57, 3) * C482;
    const double C485 = C483;
    const double C486 = -C483;
    const double C487 = -C484;
    const double C488 = (-C169 * -3. * C169 * 2 * C60) / std::pow(C60, 4);
    const double C489 = C49 * (C49 - 1) * std::pow(C59, C49 - 2) * std::pow(C168, 2);
    const double C490 = C489 / Pi;
    const double C491 = C50 * (C50 - 1) * std::pow(C67, C50 - 2) * std::pow(C173, 2) +
                        C50 * std::pow(C67, C50 - 1) * C485;
    const double C492 = C51 * (C51 - 1) * std::pow(C67, C51 - 2) * std::pow(C173, 2) +
                        C51 * std::pow(C67, C51 - 1) * C485;
    const double C493 = C50 * (C50 - 1) * std::pow(C68, C50 - 2) * std::pow(C174, 2) +
                        C50 * std::pow(C68, C50 - 1) * C486;
    const double C494 = C51 * (C51 - 1) * std::pow(C68, C51 - 2) * std::pow(C174, 2) +
                        C51 * std::pow(C68, C51 - 1) * C486;
    const double C495 = C49 * (C49 - 1) * std::pow(C70, C49 - 2) * std::pow(C176, 2) +
                        C49 * std::pow(C70, C49 - 1) * C488;
    const double C496 = C50 * (C50 - 1) * std::pow(C70, C50 - 2) * std::pow(C176, 2) +
                        C50 * std::pow(C70, C50 - 1) * C488;
    const double C497 = 0.11125 * C495;
    const double C498 = 0.20548 * C495;
    const double C499 = 0.2137 * C495;
    const double C500 = 0.49294 * C496;
    const double C501 = 0.49671 * C496;
    const double C502 = 0.62517 * C496;
    const double C503 =
        0.88026 * C488 / (2 * std::sqrt(C70)) -
        0.88026 * std::pow(C176, 2) / (std::sqrt(C70) * std::pow(2 * std::sqrt(C70), 2));
    const double C504 =
        1.6382 * C488 / (2 * std::sqrt(C70)) -
        1.6382 * std::pow(C176, 2) / (std::sqrt(C70) * std::pow(2 * std::sqrt(C70), 2));
    const double C505 =
        3.3662 * C488 / (2 * std::sqrt(C70)) -
        3.3662 * std::pow(C176, 2) / (std::sqrt(C70) * std::pow(2 * std::sqrt(C70), 2));
    const double C506 = 3.5876 * C495;
    const double C507 = 3.6231 * C495;
    const double C508 = 6.1977 * C495;
    const double C509 = C491 + C493;
    const double C510 = C492 + C494;
    const double C511 =
        7.5957 * C495 / (2 * std::sqrt(C79)) -
        7.5957 * std::pow(C183, 2) / (std::sqrt(C79) * std::pow(2 * std::sqrt(C79), 2));
    const double C512 =
        10.357 * C495 / (2 * std::sqrt(C79)) -
        10.357 * std::pow(C183, 2) / (std::sqrt(C79) * std::pow(2 * std::sqrt(C79), 2));
    const double C513 =
        0.141189e2 * C495 / (2 * std::sqrt(C79)) -
        0.141189e2 * std::pow(C183, 2) / (std::sqrt(C79) * std::pow(2 * std::sqrt(C79), 2));
    const double C514 = ((C490 * C93 + C197 * C178) / (2 * std::sqrt(C73)) +
                         (C197 / (2 * std::sqrt(C73)) -
                          C178 * C93 / (std::sqrt(C73) * std::pow(2 * std::sqrt(C73), 2))) *
                             C178) +
                        std::sqrt(C73) * C509;
    const double C515 = C497;
    const double C516 = C498;
    const double C517 = C499;
    const double C518 = C510;
    const double C519 = C509 / 2.;
    const double C520 = 2. * C514;
    const double C521 = C511 + C506;
    const double C522 = C512 + C507;
    const double C523 = C513 + C508;
    const double C524 = 6 * C105 * std::pow(C207, 2) + 3 * std::pow(C105, 2) * C519;
    const double C525 = 0.3109069086965489e-1 * C524 + 0.;
    const double C526 = (C162 * C208 + C208 * C162) + C47 * C520;
    const double C527 = C521 + C504;
    const double C528 = C522 + C503;
    const double C529 = C523 + C505;
    const double C530 = C527 + C500;
    const double C531 = C528 + C501;
    const double C532 = C529 + C502;
    const double C533 = C526 / 2.;
    const double C534 = 0.0310907 * C532;
    const double C535 = 0.0337738 * C531;
    const double C536 = 0.0621814 * C530;
    const double C537 = C533;
    const double C538 = 2. * C537;
    const double C539 = 2 * C120 * std::pow(C222, 2) / std::pow(C120, 4) - C534 / std::pow(C120, 2);
    const double C540 = 2 * C121 * std::pow(C223, 2) / std::pow(C121, 4) - C535 / std::pow(C121, 2);
    const double C541 = 2 * C122 * std::pow(C224, 2) / std::pow(C122, 4) - C536 / std::pow(C122, 2);
    const double C542 = C539;
    const double C543 = C540;
    const double C544 = C541;
    const double C545 = C226 * C226 * std::sqrt(C61) * 2 * C124 / std::pow(C124, 4) -
                        C538 * std::sqrt(C61) / std::pow(C124, 2);
    const double C546 = C542 / C128 - std::pow(C230, 2) / std::pow(C128, 2);
    const double C547 = C543 / C129 - std::pow(C231, 2) / std::pow(C129, 2);
    const double C548 = C544 / C130 - std::pow(C232, 2) / std::pow(C130, 2);
    const double C549 = 2 * std::pow(C233, 2) + 2 * C131 * C545;
    const double C550 = 0.0667245506031492 * C549;
    const double C551 = ((C203 * C235 + C133 * C515) + C235 * C203) + C101 * C547;
    const double C552 = ((C204 * C234 + C132 * C516) + C234 * C204) + C102 * C546;
    const double C553 = ((C205 * C236 + C134 * C517) + C236 * C205) + C103 * C548;
    const double C554 = 0.0310907 * C552;
    const double C555 = 0.0621814 * C553;
    const double C556 = ((C239 * C206 + C104 * C551) + C206 * C239) + C137 * C518;
    const double C557 = ((C244 * C175 + C69 * C556) + C175 * C244) + C142 * C487;
    const double C558 = C555 - C554;
    const double C559 = 0.0337738 * C557;
    const double C560 = ((C246 * C206 + C104 * C558) + C206 * C246) + C144 * C518;
    const double C561 = ((C171 * C248 + C146 * C484) + C248 * C171) + C64 * C560;
    const double C562 =
        (C559 / C66 +
         ((-C247) / std::pow(C66, 2) - (-C172 * C145 * 2 * C66) / std::pow(C66, 4)) * C172) -
        C247 * C172 / std::pow(C66, 2);
    const double C563 = C562 - C555;
    const double C564 = C561 / C65;
    const double C565 = C563 + C564;
    const double C566 =
        ((C565 / C111 +
          ((-C253) / std::pow(C111, 2) - (-C213 * C151 * 2 * C111) / std::pow(C111, 4)) * C213) -
         C525 * C151 / std::pow(C111, 2)) -
        C253 * C213 / std::pow(C111, 2);
    const double C567 = -C566;
    const double C568 = std::exp(C153) * std::pow(C255, 2) + std::exp(C153) * C567;
    const double C569 = C568;
    const double C570 = 0.3109069086965489e-1 * C569 + 0.;
    const double C571 =
        ((C550 / C156 +
          ((-C238) / std::pow(C156, 2) - (-C258 * C136 * 2 * C156) / std::pow(C156, 4)) * C258) -
         C570 * C136 / std::pow(C156, 2)) -
        C238 * C258 / std::pow(C156, 2);
    matrix.upper[0] =
        ((((((((((((0. * C162 + (0. * C47 + 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                                              (0.3109069086965489e-1 *
                                                               ((C157 + std::pow(C157, 2)) + 1.)) +
                                                          1.)) *
                                    C212) +
                   C237 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                        (0.06672455060314922 * C135 * (C157 + 1.) /
                             (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                         1.))) +
                  C259 *
                      (C47 * 0.3109069086965489e-1 * C110 *
                           (0. -
                            0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                       0. * C110 *
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                0.06672455060314922 * C135 -
                            0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                                (2 * C157 + 1))) /
                      (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.))) +
                 (((((0. * C162 + 1. * C253) +
                     (0. * C47 +
                      0.3109069086965489e-1 *
                          std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                   1.)) *
                         C212) +
                    C237 *
                        (0. * C47 +
                         0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.)) /
                        (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                         (0.06672455060314922 * C135 * (C157 + 1.) /
                              (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                          1.))) +
                   C259 *
                       (C47 * 0.3109069086965489e-1 * C110 *
                            (0. - 0.06672455060314922 *
                                      (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                        0.3109069086965489e-1 * C110 *
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                 0.06672455060314922 * C135 -
                             0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                                 (2 * C157 + 1))) /
                       (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                        (0.06672455060314922 * C135 * (C157 + 1.) /
                             (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                         1.))) +
                  0.) *
                     C162) +
                ((((1. * C162 + (0. * C47 + 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                                              (0.3109069086965489e-1 *
                                                               ((C157 + std::pow(C157, 2)) + 1.)) +
                                                          1.)) *
                                    C212) +
                   C237 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                        (0.06672455060314922 * C135 * (C157 + 1.) /
                             (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                         1.))) +
                  C259 *
                      (C47 * 0.3109069086965489e-1 * C110 *
                           (0. -
                            0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                       0. * C110 *
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                0.06672455060314922 * C135 -
                            0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                                (2 * C157 + 1))) /
                      (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.))) +
                 0.) *
                    C253) +
               (((((0.3109069086965489e-1 *
                        std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                 1.) +
                    0.) *
                       C162 +
                   (0. * C47 +
                    0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                  1.)) *
                       C212) +
                  C237 *
                      (C47 * 0.2074512376219157e-2 * (C157 + 1.) +
                       0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.))) +
                 C259 *
                     (C47 * 0.3109069086965489e-1 *
                          (C110 * (0. - 0.06672455060314922 *
                                            (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                0.06672455060314922 * C135 -
                            0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                                (2 * C157 + 1))) +
                      0. * C110 *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1))) /
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.))) +
                0.) *
                   C212) +
              (((((0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                        (0.06672455060314922 * C135 * (C157 + 1.) /
                             (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                         1.)) +
                   0.) *
                      C162 +
                  (C47 * 0.2074512376219157e-2 * (C157 + 1.) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                        (0.06672455060314922 * C135 * (C157 + 1.) /
                             (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                         1.)) +
                   0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                 1.)) *
                      C212) +
                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.) *
                      C237 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) -
                  C237 * C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.) *
                      0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                      0.06672455060314922 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.))) /
                     std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                  (0.06672455060314922 * C135 * (C157 + 1.) /
                                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                   1.),
                              2)) +
                (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                     (0.06672455060314922 * C135 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      1.) *
                     C259 *
                     (C47 * 0.3109069086965489e-1 * C110 *
                          (0.2074512376219157e-2 * ((C157 + std::pow(C157, 2)) + 1.) -
                           0.06672455060314922 *
                               (0. * C135 * (C157 + 1.) +
                                (C157 + 1.) * 0.3109069086965489e-1 * (2 * C157 + 1))) +
                      0. * C110 *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1))) -
                 C259 * C47 * 0.3109069086965489e-1 * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1)) *
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                          0.06672455060314922 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      0. * ((C157 + std::pow(C157, 2)) + 1.) *
                          (0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.))) /
                    std::pow(
                        std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                            (0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.),
                        2)) +
               0.) *
                  C237) +
             (((((0.3109069086965489e-1 * C110 *
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                           0.06672455060314922 * C135 -
                       0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                           (2 * C157 + 1)) /
                      (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.)) +
                  0.) *
                     C162 +
                 (C47 * 0.3109069086965489e-1 *
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                           0.06672455060314922 * C135 -
                       0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                           (2 * C157 + 1)) /
                      (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.)) +
                  0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                    (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                1.)) *
                     C212) +
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                     (0.06672455060314922 * C135 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      1.) *
                     C237 *
                     (C47 * 0.2074512376219157e-2 * C110 +
                      0. * C110 * 0.06672455060314922 * (C157 + 1.)) -
                 C237 * C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.) *
                     0.3109069086965489e-1 *
                     (((C157 + std::pow(C157, 2)) + 1.) *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1)) /
                          std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) +
                      (2 * C157 + 1) *
                          (0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.))) /
                    std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                 (0.06672455060314922 * C135 * (C157 + 1.) /
                                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                  1.),
                             2)) +
               (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                    (0.06672455060314922 * C135 * (C157 + 1.) /
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                     1.) *
                    C259 *
                    (C47 * 0.3109069086965489e-1 * C110 *
                         (0.3109069086965489e-1 * (2 * C157 + 1) * 0.06672455060314922 * C135 -
                          0.06672455060314922 * (0.6218138173930979e-1 * C135 * (C157 + 1.) +
                                                 C135 * 0.3109069086965489e-1 * (2 * C157 + 1))) +
                     0. * C110 *
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                              0.06672455060314922 * C135 -
                          0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                              (2 * C157 + 1))) -
                C259 * C47 * 0.3109069086965489e-1 * C110 *
                    (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                         0.06672455060314922 * C135 -
                     0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                         (2 * C157 + 1)) *
                    (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                              0.06672455060314922 * C135 -
                          0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                              (2 * C157 + 1)) /
                         std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) +
                     0.6218138173930979e-1 * (2 * C157 + 1) * 0.3109069086965489e-1 *
                         ((C157 + std::pow(C157, 2)) + 1.) *
                         (0.06672455060314922 * C135 * (C157 + 1.) /
                              (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                          1.))) /
                   std::pow(std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                                (0.06672455060314922 * C135 * (C157 + 1.) /
                                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                 1.),
                            2)) +
              0.) *
                 C259) +
            (((((0. * C162 + 1. * C47) +
                (0. * C47 +
                 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                               1.)) *
                    C212) +
               C237 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                    (0.06672455060314922 * C135 * (C157 + 1.) /
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                     1.))) +
              C259 *
                  (C47 * 0.3109069086965489e-1 * C110 *
                       (0. -
                        0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                   0. * C110 *
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                            0.06672455060314922 * C135 -
                        0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                            (2 * C157 + 1))) /
                  (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                   (0.06672455060314922 * C135 * (C157 + 1.) /
                        (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                    1.))) +
             0.) *
                C565) +
           ((((0. * C162 +
               (C47 * 0.3109069086965489e-1 *
                    std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.) +
                (0. * C47 +
                 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                               1.)) *
                    C212)) +
              C237 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                  (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                   (0.06672455060314922 * C135 * (C157 + 1.) /
                        (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                    1.))) +
             C259 *
                 (C47 * 0.3109069086965489e-1 * C110 *
                      (0. - 0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                  0. * C110 *
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                           0.06672455060314922 * C135 -
                       0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                           (2 * C157 + 1))) /
                 (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                  (0.06672455060314922 * C135 * (C157 + 1.) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                   1.))) +
            0.) *
               C524) +
          ((((0. * C162 + (0. * C47 + 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                                        (0.3109069086965489e-1 *
                                                         ((C157 + std::pow(C157, 2)) + 1.)) +
                                                    1.)) *
                              C212) +
             (C237 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) +
              C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.)) /
                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                  (0.06672455060314922 * C135 * (C157 + 1.) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                   1.))) +
            C259 *
                (C47 * 0.3109069086965489e-1 * C110 *
                     (0. - 0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                 0. * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1))) /
                (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                 (0.06672455060314922 * C135 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                  1.))) +
           0.) *
              C549) +
         ((((0. * C162 + (0. * C47 + 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                                       (0.3109069086965489e-1 *
                                                        ((C157 + std::pow(C157, 2)) + 1.)) +
                                                   1.)) *
                             C212) +
            C237 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                 (0.06672455060314922 * C135 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                  1.))) +
           (C259 *
                (C47 * 0.3109069086965489e-1 * C110 *
                     (0. - 0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                 0. * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1))) +
            C47 * 0.3109069086965489e-1 * C110 *
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) * 0.06672455060314922 *
                     C135 -
                 0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                     (2 * C157 + 1))) /
               (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                (0.06672455060314922 * C135 * (C157 + 1.) /
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                 1.))) +
          0.) *
             C571) +
        0.;

    const double C260 = 1;
    const double C261 = -1;
    const double C262 = 0.9999999999999999 * C261 + 0.;
    const double C263 = C52 * C260;
    const double C264 = Pi * C260;
    const double C265 = C261 / C47 - C260 * C48 / std::pow(C47, 2);
    const double C266 = 3. * C263;
    const double C267 = 4. * C264;
    const double C268 = C262 / C47 - C260 * C53 / std::pow(C47, 2);
    const double C269 = 4 * std::pow(C57, 3) * C265;
    const double C270 = 0.;
    const double C271 = C268;
    const double C272 = -C268;
    const double C273 = -C269;
    const double C274 = -3. * C267 / std::pow(C60, 2);
    const double C275 = C49 * std::pow(C59, C49 - 1) * C266;
    const double C276 = C275 / Pi;
    const double C277 = C50 * std::pow(C67, C50 - 1) * C271;
    const double C278 = C51 * std::pow(C67, C51 - 1) * C271;
    const double C279 = C50 * std::pow(C68, C50 - 1) * C272;
    const double C280 = C51 * std::pow(C68, C51 - 1) * C272;
    const double C281 = C49 * std::pow(C70, C49 - 1) * C274;
    const double C282 = C50 * std::pow(C70, C50 - 1) * C274;
    const double C283 = 0.11125 * C281;
    const double C284 = 0.20548 * C281;
    const double C285 = 0.2137 * C281;
    const double C286 = 0.49294 * C282;
    const double C287 = 0.49671 * C282;
    const double C288 = 0.62517 * C282;
    const double C289 = 0.88026 * C274 / (2 * std::sqrt(C70));
    const double C290 = 1.6382 * C274 / (2 * std::sqrt(C70));
    const double C291 = 3.3662 * C274 / (2 * std::sqrt(C70));
    const double C292 = 3.5876 * C281;
    const double C293 = 3.6231 * C281;
    const double C294 = 6.1977 * C281;
    const double C295 = C277 + C279;
    const double C296 = C278 + C280;
    const double C297 = 7.5957 * C281 / (2 * std::sqrt(C79));
    const double C298 = 10.357 * C281 / (2 * std::sqrt(C79));
    const double C299 = 0.141189e2 * C281 / (2 * std::sqrt(C79));
    const double C300 = C276 * C93 / (2 * std::sqrt(C73)) + std::sqrt(C73) * C295;
    const double C301 = C283;
    const double C302 = C284;
    const double C303 = C285;
    const double C304 = C296;
    const double C305 = C295 / 2.;
    const double C306 = 2. * C300;
    const double C307 = C297 + C292;
    const double C308 = C298 + C293;
    const double C309 = C299 + C294;
    const double C310 = 3 * std::pow(C105, 2) * C305;
    const double C311 = 0.3109069086965489e-1 * C310 + 0.;
    const double C312 = C106 * C260 + C47 * C306;
    const double C313 = C307 + C290;
    const double C314 = C308 + C289;
    const double C315 = C309 + C291;
    const double C316 = C313 + C286;
    const double C317 = C314 + C287;
    const double C318 = C315 + C288;
    const double C319 = C312 / 2.;
    const double C320 = 0.0310907 * C318;
    const double C321 = 0.0337738 * C317;
    const double C322 = 0.0621814 * C316;
    const double C323 = C319;
    const double C324 = 2. * C323;
    const double C325 = (-C320) / std::pow(C120, 2);
    const double C326 = (-C321) / std::pow(C121, 2);
    const double C327 = (-C322) / std::pow(C122, 2);
    const double C328 = C325;
    const double C329 = C326;
    const double C330 = C327;
    const double C331 = (-C324 * std::sqrt(C61)) / std::pow(C124, 2);
    const double C332 = C328 / C128;
    const double C333 = C329 / C129;
    const double C334 = C330 / C130;
    const double C335 = 2 * C131 * C331;
    const double C336 = 0.0667245506031492 * C335;
    const double C337 = C133 * C301 + C101 * C333;
    const double C338 = C132 * C302 + C102 * C332;
    const double C339 = C134 * C303 + C103 * C334;
    const double C340 = 0.0310907 * C338;
    const double C341 = 0.0621814 * C339;
    const double C342 = C104 * C337 + C137 * C304;
    const double C343 = C69 * C342 + C142 * C273;
    const double C344 = C341 - C340;
    const double C345 = 0.0337738 * C343;
    const double C346 = C104 * C344 + C144 * C304;
    const double C347 = C146 * C269 + C64 * C346;
    const double C348 = C345 / C66 - C270 * C145 / std::pow(C66, 2);
    const double C349 = C348 - C341;
    const double C350 = C347 / C65;
    const double C351 = C349 + C350;
    const double C352 = C351 / C111 - C311 * C151 / std::pow(C111, 2);
    const double C353 = -C352;
    const double C354 = std::exp(C153) * C353;
    const double C355 = C354;
    const double C356 = 0.3109069086965489e-1 * C355 + 0.;
    const double C357 = C336 / C156 - C356 * C136 / std::pow(C156, 2);
    const double C572 = 0.;
    const double C573 =
        ((-C163) / std::pow(C47, 2) - (-C162 * C48 * 2 * C47) / std::pow(C47, 4)) * C260 -
        C261 * C162 / std::pow(C47, 2);
    const double C574 =
        (C572 / C47 +
         ((-C164) / std::pow(C47, 2) - (-C162 * C53 * 2 * C47) / std::pow(C47, 4)) * C260) -
        C262 * C162 / std::pow(C47, 2);
    const double C575 = 12 * std::pow(C57, 2) * C167 * C265 + 4 * std::pow(C57, 3) * C573;
    const double C576 = C574;
    const double C577 = -C574;
    const double C578 = -C575;
    const double C579 = (-C267 * -3. * C169 * 2 * C60) / std::pow(C60, 4);
    const double C580 = C49 * (C49 - 1) * std::pow(C59, C49 - 2) * C168 * C266;
    const double C581 = C580 / Pi;
    const double C582 = C50 * (C50 - 1) * std::pow(C67, C50 - 2) * C173 * C271 +
                        C50 * std::pow(C67, C50 - 1) * C576;
    const double C583 = C51 * (C51 - 1) * std::pow(C67, C51 - 2) * C173 * C271 +
                        C51 * std::pow(C67, C51 - 1) * C576;
    const double C584 = C50 * (C50 - 1) * std::pow(C68, C50 - 2) * C174 * C272 +
                        C50 * std::pow(C68, C50 - 1) * C577;
    const double C585 = C51 * (C51 - 1) * std::pow(C68, C51 - 2) * C174 * C272 +
                        C51 * std::pow(C68, C51 - 1) * C577;
    const double C586 = C49 * (C49 - 1) * std::pow(C70, C49 - 2) * C176 * C274 +
                        C49 * std::pow(C70, C49 - 1) * C579;
    const double C587 = C50 * (C50 - 1) * std::pow(C70, C50 - 2) * C176 * C274 +
                        C50 * std::pow(C70, C50 - 1) * C579;
    const double C588 = 0.11125 * C586;
    const double C589 = 0.20548 * C586;
    const double C590 = 0.2137 * C586;
    const double C591 = 0.49294 * C587;
    const double C592 = 0.49671 * C587;
    const double C593 = 0.62517 * C587;
    const double C594 = 0.88026 * C579 / (2 * std::sqrt(C70)) -
                        C274 * 0.88026 * C176 / (std::sqrt(C70) * std::pow(2 * std::sqrt(C70), 2));
    const double C595 = 1.6382 * C579 / (2 * std::sqrt(C70)) -
                        C274 * 1.6382 * C176 / (std::sqrt(C70) * std::pow(2 * std::sqrt(C70), 2));
    const double C596 = 3.3662 * C579 / (2 * std::sqrt(C70)) -
                        C274 * 3.3662 * C176 / (std::sqrt(C70) * std::pow(2 * std::sqrt(C70), 2));
    const double C597 = 3.5876 * C586;
    const double C598 = 3.6231 * C586;
    const double C599 = 6.1977 * C586;
    const double C600 = C582 + C584;
    const double C601 = C583 + C585;
    const double C602 = 7.5957 * C586 / (2 * std::sqrt(C79)) -
                        C281 * 7.5957 * C183 / (std::sqrt(C79) * std::pow(2 * std::sqrt(C79), 2));
    const double C603 = 10.357 * C586 / (2 * std::sqrt(C79)) -
                        C281 * 10.357 * C183 / (std::sqrt(C79) * std::pow(2 * std::sqrt(C79), 2));
    const double C604 =
        0.141189e2 * C586 / (2 * std::sqrt(C79)) -
        C281 * 0.141189e2 * C183 / (std::sqrt(C79) * std::pow(2 * std::sqrt(C79), 2));
    const double C605 = ((C581 * C93 + C295 * C178) / (2 * std::sqrt(C73)) +
                         (C197 / (2 * std::sqrt(C73)) -
                          C178 * C93 / (std::sqrt(C73) * std::pow(2 * std::sqrt(C73), 2))) *
                             C276) +
                        std::sqrt(C73) * C600;
    const double C606 = C588;
    const double C607 = C589;
    const double C608 = C590;
    const double C609 = C601;
    const double C610 = C600 / 2.;
    const double C611 = 2. * C605;
    const double C612 = C602 + C597;
    const double C613 = C603 + C598;
    const double C614 = C604 + C599;
    const double C615 = 6 * C105 * C207 * C305 + 3 * std::pow(C105, 2) * C610;
    const double C616 = 0.3109069086965489e-1 * C615 + 0.;
    const double C617 = (C162 * C306 + C208 * C260) + C47 * C611;
    const double C618 = C612 + C595;
    const double C619 = C613 + C594;
    const double C620 = C614 + C596;
    const double C621 = C618 + C591;
    const double C622 = C619 + C592;
    const double C623 = C620 + C593;
    const double C624 = C617 / 2.;
    const double C625 = 0.0310907 * C623;
    const double C626 = 0.0337738 * C622;
    const double C627 = 0.0621814 * C621;
    const double C628 = C624;
    const double C629 = 2. * C628;
    const double C630 = C320 * C222 * 2 * C120 / std::pow(C120, 4) - C625 / std::pow(C120, 2);
    const double C631 = C321 * C223 * 2 * C121 / std::pow(C121, 4) - C626 / std::pow(C121, 2);
    const double C632 = C322 * C224 * 2 * C122 / std::pow(C122, 4) - C627 / std::pow(C122, 2);
    const double C633 = C630;
    const double C634 = C631;
    const double C635 = C632;
    const double C636 = C324 * C226 * std::sqrt(C61) * 2 * C124 / std::pow(C124, 4) -
                        C629 * std::sqrt(C61) / std::pow(C124, 2);
    const double C637 = C633 / C128 - C328 * C230 / std::pow(C128, 2);
    const double C638 = C634 / C129 - C329 * C231 / std::pow(C129, 2);
    const double C639 = C635 / C130 - C330 * C232 / std::pow(C130, 2);
    const double C640 = 2 * C233 * C331 + 2 * C131 * C636;
    const double C641 = 0.0667245506031492 * C640;
    const double C642 = ((C203 * C333 + C133 * C606) + C235 * C301) + C101 * C638;
    const double C643 = ((C204 * C332 + C132 * C607) + C234 * C302) + C102 * C637;
    const double C644 = ((C205 * C334 + C134 * C608) + C236 * C303) + C103 * C639;
    const double C645 = 0.0310907 * C643;
    const double C646 = 0.0621814 * C644;
    const double C647 = ((C239 * C304 + C104 * C642) + C206 * C337) + C137 * C609;
    const double C648 = ((C244 * C273 + C69 * C647) + C175 * C342) + C142 * C578;
    const double C649 = C646 - C645;
    const double C650 = 0.0337738 * C648;
    const double C651 = ((C246 * C304 + C104 * C649) + C206 * C344) + C144 * C609;
    const double C652 = ((C171 * C346 + C146 * C575) + C248 * C269) + C64 * C651;
    const double C653 =
        (C650 / C66 +
         ((-C247) / std::pow(C66, 2) - (-C172 * C145 * 2 * C66) / std::pow(C66, 4)) * C270) -
        C345 * C172 / std::pow(C66, 2);
    const double C654 = C653 - C646;
    const double C655 = C652 / C65;
    const double C656 = C654 + C655;
    const double C657 =
        ((C656 / C111 +
          ((-C253) / std::pow(C111, 2) - (-C213 * C151 * 2 * C111) / std::pow(C111, 4)) * C311) -
         C616 * C151 / std::pow(C111, 2)) -
        C351 * C213 / std::pow(C111, 2);
    const double C658 = -C657;
    const double C659 = std::exp(C153) * C255 * C353 + std::exp(C153) * C658;
    const double C660 = C659;
    const double C661 = 0.3109069086965489e-1 * C660 + 0.;
    const double C662 =
        ((C641 / C156 +
          ((-C238) / std::pow(C156, 2) - (-C258 * C136 * 2 * C156) / std::pow(C156, 4)) * C356) -
         C661 * C136 / std::pow(C156, 2)) -
        C336 * C258 / std::pow(C156, 2);
    matrix.upper[1] =
        ((((((((((((0. * C162 + (0. * C47 + 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                                              (0.3109069086965489e-1 *
                                                               ((C157 + std::pow(C157, 2)) + 1.)) +
                                                          1.)) *
                                    C212) +
                   C237 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                        (0.06672455060314922 * C135 * (C157 + 1.) /
                             (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                         1.))) +
                  C259 *
                      (C47 * 0.3109069086965489e-1 * C110 *
                           (0. -
                            0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                       0. * C110 *
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                0.06672455060314922 * C135 -
                            0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                                (2 * C157 + 1))) /
                      (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.))) +
                 (((((0. * C162 + 1. * C253) +
                     (0. * C47 +
                      0.3109069086965489e-1 *
                          std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                   1.)) *
                         C212) +
                    C237 *
                        (0. * C47 +
                         0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.)) /
                        (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                         (0.06672455060314922 * C135 * (C157 + 1.) /
                              (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                          1.))) +
                   C259 *
                       (C47 * 0.3109069086965489e-1 * C110 *
                            (0. - 0.06672455060314922 *
                                      (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                        0.3109069086965489e-1 * C110 *
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                 0.06672455060314922 * C135 -
                             0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                                 (2 * C157 + 1))) /
                       (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                        (0.06672455060314922 * C135 * (C157 + 1.) /
                             (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                         1.))) +
                  0.) *
                     C260) +
                ((((1. * C162 + (0. * C47 + 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                                              (0.3109069086965489e-1 *
                                                               ((C157 + std::pow(C157, 2)) + 1.)) +
                                                          1.)) *
                                    C212) +
                   C237 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                        (0.06672455060314922 * C135 * (C157 + 1.) /
                             (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                         1.))) +
                  C259 *
                      (C47 * 0.3109069086965489e-1 * C110 *
                           (0. -
                            0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                       0. * C110 *
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                0.06672455060314922 * C135 -
                            0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                                (2 * C157 + 1))) /
                      (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.))) +
                 0.) *
                    C351) +
               (((((0.3109069086965489e-1 *
                        std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                 1.) +
                    0.) *
                       C162 +
                   (0. * C47 +
                    0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                  1.)) *
                       C212) +
                  C237 *
                      (C47 * 0.2074512376219157e-2 * (C157 + 1.) +
                       0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.))) +
                 C259 *
                     (C47 * 0.3109069086965489e-1 *
                          (C110 * (0. - 0.06672455060314922 *
                                            (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                0.06672455060314922 * C135 -
                            0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                                (2 * C157 + 1))) +
                      0. * C110 *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1))) /
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.))) +
                0.) *
                   C310) +
              (((((0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                        (0.06672455060314922 * C135 * (C157 + 1.) /
                             (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                         1.)) +
                   0.) *
                      C162 +
                  (C47 * 0.2074512376219157e-2 * (C157 + 1.) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                        (0.06672455060314922 * C135 * (C157 + 1.) /
                             (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                         1.)) +
                   0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                 1.)) *
                      C212) +
                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.) *
                      C237 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) -
                  C237 * C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.) *
                      0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                      0.06672455060314922 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.))) /
                     std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                  (0.06672455060314922 * C135 * (C157 + 1.) /
                                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                   1.),
                              2)) +
                (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                     (0.06672455060314922 * C135 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      1.) *
                     C259 *
                     (C47 * 0.3109069086965489e-1 * C110 *
                          (0.2074512376219157e-2 * ((C157 + std::pow(C157, 2)) + 1.) -
                           0.06672455060314922 *
                               (0. * C135 * (C157 + 1.) +
                                (C157 + 1.) * 0.3109069086965489e-1 * (2 * C157 + 1))) +
                      0. * C110 *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1))) -
                 C259 * C47 * 0.3109069086965489e-1 * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1)) *
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                          0.06672455060314922 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      0. * ((C157 + std::pow(C157, 2)) + 1.) *
                          (0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.))) /
                    std::pow(
                        std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                            (0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.),
                        2)) +
               0.) *
                  C335) +
             (((((0.3109069086965489e-1 * C110 *
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                           0.06672455060314922 * C135 -
                       0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                           (2 * C157 + 1)) /
                      (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.)) +
                  0.) *
                     C162 +
                 (C47 * 0.3109069086965489e-1 *
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                           0.06672455060314922 * C135 -
                       0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                           (2 * C157 + 1)) /
                      (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.)) +
                  0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                    (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                1.)) *
                     C212) +
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                     (0.06672455060314922 * C135 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      1.) *
                     C237 *
                     (C47 * 0.2074512376219157e-2 * C110 +
                      0. * C110 * 0.06672455060314922 * (C157 + 1.)) -
                 C237 * C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.) *
                     0.3109069086965489e-1 *
                     (((C157 + std::pow(C157, 2)) + 1.) *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1)) /
                          std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) +
                      (2 * C157 + 1) *
                          (0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.))) /
                    std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                 (0.06672455060314922 * C135 * (C157 + 1.) /
                                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                  1.),
                             2)) +
               (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                    (0.06672455060314922 * C135 * (C157 + 1.) /
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                     1.) *
                    C259 *
                    (C47 * 0.3109069086965489e-1 * C110 *
                         (0.3109069086965489e-1 * (2 * C157 + 1) * 0.06672455060314922 * C135 -
                          0.06672455060314922 * (0.6218138173930979e-1 * C135 * (C157 + 1.) +
                                                 C135 * 0.3109069086965489e-1 * (2 * C157 + 1))) +
                     0. * C110 *
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                              0.06672455060314922 * C135 -
                          0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                              (2 * C157 + 1))) -
                C259 * C47 * 0.3109069086965489e-1 * C110 *
                    (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                         0.06672455060314922 * C135 -
                     0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                         (2 * C157 + 1)) *
                    (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                              0.06672455060314922 * C135 -
                          0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                              (2 * C157 + 1)) /
                         std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) +
                     0.6218138173930979e-1 * (2 * C157 + 1) * 0.3109069086965489e-1 *
                         ((C157 + std::pow(C157, 2)) + 1.) *
                         (0.06672455060314922 * C135 * (C157 + 1.) /
                              (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                          1.))) /
                   std::pow(std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                                (0.06672455060314922 * C135 * (C157 + 1.) /
                                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                 1.),
                            2)) +
              0.) *
                 C357) +
            (((((0. * C162 + 1. * C47) +
                (0. * C47 +
                 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                               1.)) *
                    C212) +
               C237 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                    (0.06672455060314922 * C135 * (C157 + 1.) /
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                     1.))) +
              C259 *
                  (C47 * 0.3109069086965489e-1 * C110 *
                       (0. -
                        0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                   0. * C110 *
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                            0.06672455060314922 * C135 -
                        0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                            (2 * C157 + 1))) /
                  (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                   (0.06672455060314922 * C135 * (C157 + 1.) /
                        (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                    1.))) +
             0.) *
                C656) +
           ((((0. * C162 +
               (C47 * 0.3109069086965489e-1 *
                    std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.) +
                (0. * C47 +
                 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                               1.)) *
                    C212)) +
              C237 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                  (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                   (0.06672455060314922 * C135 * (C157 + 1.) /
                        (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                    1.))) +
             C259 *
                 (C47 * 0.3109069086965489e-1 * C110 *
                      (0. - 0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                  0. * C110 *
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                           0.06672455060314922 * C135 -
                       0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                           (2 * C157 + 1))) /
                 (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                  (0.06672455060314922 * C135 * (C157 + 1.) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                   1.))) +
            0.) *
               C615) +
          ((((0. * C162 + (0. * C47 + 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                                        (0.3109069086965489e-1 *
                                                         ((C157 + std::pow(C157, 2)) + 1.)) +
                                                    1.)) *
                              C212) +
             (C237 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) +
              C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.)) /
                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                  (0.06672455060314922 * C135 * (C157 + 1.) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                   1.))) +
            C259 *
                (C47 * 0.3109069086965489e-1 * C110 *
                     (0. - 0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                 0. * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1))) /
                (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                 (0.06672455060314922 * C135 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                  1.))) +
           0.) *
              C640) +
         ((((0. * C162 + (0. * C47 + 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                                       (0.3109069086965489e-1 *
                                                        ((C157 + std::pow(C157, 2)) + 1.)) +
                                                   1.)) *
                             C212) +
            C237 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                 (0.06672455060314922 * C135 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                  1.))) +
           (C259 *
                (C47 * 0.3109069086965489e-1 * C110 *
                     (0. - 0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                 0. * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1))) +
            C47 * 0.3109069086965489e-1 * C110 *
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) * 0.06672455060314922 *
                     C135 -
                 0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                     (2 * C157 + 1))) /
               (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                (0.06672455060314922 * C135 * (C157 + 1.) /
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                 1.))) +
          0.) *
             C662) +
        0.;

    const double C358 = 0.;
    const double C359 = 1;
    const double C360 = C359;
    const double C361 = C358 / C47;
    const double C362 = 0.;
    const double C363 = C361;
    const double C364 = -C361;
    const double C365 = C50 * std::pow(C67, C50 - 1) * C363;
    const double C366 = C51 * std::pow(C67, C51 - 1) * C363;
    const double C367 = C50 * std::pow(C68, C50 - 1) * C364;
    const double C368 = C51 * std::pow(C68, C51 - 1) * C364;
    const double C369 = C365 + C367;
    const double C370 = C366 + C368;
    const double C371 = std::sqrt(C73) * C369;
    const double C372 = C370;
    const double C373 = C369 / 2.;
    const double C374 = 2. * C371;
    const double C375 = 3 * std::pow(C105, 2) * C373;
    const double C376 = 0.3109069086965489e-1 * C375 + 0.;
    const double C377 = C47 * C374;
    const double C378 = C377 / 2.;
    const double C379 = C378;
    const double C380 = 2. * C379;
    const double C381 =
        C360 / (2 * std::sqrt(C61) * C124) - C380 * std::sqrt(C61) / std::pow(C124, 2);
    const double C382 = 2 * C131 * C381;
    const double C383 = 0.0667245506031492 * C382;
    const double C384 = C137 * C372;
    const double C385 = C69 * C384;
    const double C386 = 0.0337738 * C385;
    const double C387 = C144 * C372;
    const double C388 = C64 * C387;
    const double C389 = C386 / C66 - C362 * C145 / std::pow(C66, 2);
    const double C390 = C389;
    const double C391 = C388 / C65;
    const double C392 = C390 + C391;
    const double C393 = C392 / C111 - C376 * C151 / std::pow(C111, 2);
    const double C394 = -C393;
    const double C395 = std::exp(C153) * C394;
    const double C396 = C395;
    const double C397 = 0.3109069086965489e-1 * C396 + 0.;
    const double C398 = C383 / C156 - C397 * C136 / std::pow(C156, 2);
    const double C663 = (-C162 * C358) / std::pow(C47, 2);
    const double C664 = C663;
    const double C665 = -C663;
    const double C666 = C50 * (C50 - 1) * std::pow(C67, C50 - 2) * C363 * C173 +
                        C50 * std::pow(C67, C50 - 1) * C664;
    const double C667 = C51 * (C51 - 1) * std::pow(C67, C51 - 2) * C363 * C173 +
                        C51 * std::pow(C67, C51 - 1) * C664;
    const double C668 = C50 * (C50 - 1) * std::pow(C68, C50 - 2) * C364 * C174 +
                        C50 * std::pow(C68, C50 - 1) * C665;
    const double C669 = C51 * (C51 - 1) * std::pow(C68, C51 - 2) * C364 * C174 +
                        C51 * std::pow(C68, C51 - 1) * C665;
    const double C670 = C666 + C668;
    const double C671 = C667 + C669;
    const double C672 = C178 * C369 / (2 * std::sqrt(C73)) + std::sqrt(C73) * C670;
    const double C673 = C671;
    const double C674 = C670 / 2.;
    const double C675 = 2. * C672;
    const double C676 = 6 * C105 * C373 * C207 + 3 * std::pow(C105, 2) * C674;
    const double C677 = 0.3109069086965489e-1 * C676 + 0.;
    const double C678 = C374 * C162 + C47 * C675;
    const double C679 = C678 / 2.;
    const double C680 = C679;
    const double C681 = 2. * C680;
    const double C682 = ((-C360 * 2 * std::sqrt(C61)) / std::pow(2 * std::sqrt(C61) * C124, 2) -
                         (-C380 * std::sqrt(C61) * 2 * C124) / std::pow(C124, 4)) *
                            C226 -
                        C681 * std::sqrt(C61) / std::pow(C124, 2);
    const double C683 = 2 * C381 * C233 + 2 * C131 * C682;
    const double C684 = 0.0667245506031492 * C683;
    const double C685 = C372 * C239 + C137 * C673;
    const double C686 = C384 * C175 + C69 * C685;
    const double C687 = 0.0337738 * C686;
    const double C688 = C372 * C246 + C144 * C673;
    const double C689 = C387 * C171 + C64 * C688;
    const double C690 =
        (C687 / C66 +
         ((-C386) / std::pow(C66, 2) - (-C362 * C145 * 2 * C66) / std::pow(C66, 4)) * C172) -
        C247 * C362 / std::pow(C66, 2);
    const double C691 = C690;
    const double C692 = C689 / C65;
    const double C693 = C691 + C692;
    const double C694 =
        ((C693 / C111 +
          ((-C392) / std::pow(C111, 2) - (-C376 * C151 * 2 * C111) / std::pow(C111, 4)) * C213) -
         C677 * C151 / std::pow(C111, 2)) -
        C253 * C376 / std::pow(C111, 2);
    const double C695 = -C694;
    const double C696 = std::exp(C153) * C394 * C255 + std::exp(C153) * C695;
    const double C697 = C696;
    const double C698 = 0.3109069086965489e-1 * C697 + 0.;
    const double C699 =
        ((C684 / C156 +
          ((-C383) / std::pow(C156, 2) - (-C397 * C136 * 2 * C156) / std::pow(C156, 4)) * C258) -
         C698 * C136 / std::pow(C156, 2)) -
        C238 * C397 / std::pow(C156, 2);
    matrix.upper[2] =
        (((((((((((0. * C47 +
                   0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                 1.)) *
                      C375 +
                  C382 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.))) +
                 C398 *
                     (C47 * 0.3109069086965489e-1 * C110 *
                          (0. -
                           0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                      0. * C110 *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1))) /
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.))) +
                ((((1. * C392 + (0. * C47 + 0.3109069086965489e-1 *
                                                std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                                             (0.3109069086965489e-1 *
                                                              ((C157 + std::pow(C157, 2)) + 1.)) +
                                                         1.)) *
                                    C375) +
                   C382 *
                       (0. * C47 +
                        0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.)) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                        (0.06672455060314922 * C135 * (C157 + 1.) /
                             (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                         1.))) +
                  C398 *
                      (C47 * 0.3109069086965489e-1 * C110 *
                           (0. -
                            0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                       0.3109069086965489e-1 * C110 *
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                0.06672455060314922 * C135 -
                            0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                                (2 * C157 + 1))) /
                      (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.))) +
                 0.) *
                    C162) +
               ((((1. * C47 + (0. * C47 + 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                                            (0.3109069086965489e-1 *
                                                             ((C157 + std::pow(C157, 2)) + 1.)) +
                                                        1.)) *
                                  C375) +
                  C382 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.))) +
                 C398 *
                     (C47 * 0.3109069086965489e-1 * C110 *
                          (0. -
                           0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                      0. * C110 *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1))) /
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.))) +
                0.) *
                   C693) +
              ((((C47 * 0.2074512376219157e-2 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.)) +
                  0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                    (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                1.)) *
                     C375 +
                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.) *
                      C382 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) -
                  C382 * C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.) *
                      0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                      0.06672455060314922 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.))) /
                     std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                  (0.06672455060314922 * C135 * (C157 + 1.) /
                                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                   1.),
                              2)) +
                (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                     (0.06672455060314922 * C135 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      1.) *
                     C398 *
                     (C47 * 0.3109069086965489e-1 * C110 *
                          (0.2074512376219157e-2 * ((C157 + std::pow(C157, 2)) + 1.) -
                           0.06672455060314922 *
                               (0. * C135 * (C157 + 1.) +
                                (C157 + 1.) * 0.3109069086965489e-1 * (2 * C157 + 1))) +
                      0. * C110 *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1))) -
                 C398 * C47 * 0.3109069086965489e-1 * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1)) *
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                          0.06672455060314922 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      0. * ((C157 + std::pow(C157, 2)) + 1.) *
                          (0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.))) /
                    std::pow(
                        std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                            (0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.),
                        2)) +
               0.) *
                  C237) +
             ((((C47 * 0.3109069086965489e-1 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1)) /
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.)) +
                 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                               1.)) *
                    C375 +
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                     (0.06672455060314922 * C135 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      1.) *
                     C382 *
                     (C47 * 0.2074512376219157e-2 * C110 +
                      0. * C110 * 0.06672455060314922 * (C157 + 1.)) -
                 C382 * C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.) *
                     0.3109069086965489e-1 *
                     (((C157 + std::pow(C157, 2)) + 1.) *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1)) /
                          std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) +
                      (2 * C157 + 1) *
                          (0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.))) /
                    std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                 (0.06672455060314922 * C135 * (C157 + 1.) /
                                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                  1.),
                             2)) +
               (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                    (0.06672455060314922 * C135 * (C157 + 1.) /
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                     1.) *
                    C398 *
                    (C47 * 0.3109069086965489e-1 * C110 *
                         (0.3109069086965489e-1 * (2 * C157 + 1) * 0.06672455060314922 * C135 -
                          0.06672455060314922 * (0.6218138173930979e-1 * C135 * (C157 + 1.) +
                                                 C135 * 0.3109069086965489e-1 * (2 * C157 + 1))) +
                     0. * C110 *
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                              0.06672455060314922 * C135 -
                          0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                              (2 * C157 + 1))) -
                C398 * C47 * 0.3109069086965489e-1 * C110 *
                    (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                         0.06672455060314922 * C135 -
                     0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                         (2 * C157 + 1)) *
                    (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                              0.06672455060314922 * C135 -
                          0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                              (2 * C157 + 1)) /
                         std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) +
                     0.6218138173930979e-1 * (2 * C157 + 1) * 0.3109069086965489e-1 *
                         ((C157 + std::pow(C157, 2)) + 1.) *
                         (0.06672455060314922 * C135 * (C157 + 1.) /
                              (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                          1.))) /
                   std::pow(std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                                (0.06672455060314922 * C135 * (C157 + 1.) /
                                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                 1.),
                            2)) +
              0.) *
                 C259) +
            ((((C47 * 0.3109069086965489e-1 *
                    std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.) +
                (0. * C47 +
                 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                               1.)) *
                    C375) +
               C382 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                    (0.06672455060314922 * C135 * (C157 + 1.) /
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                     1.))) +
              C398 *
                  (C47 * 0.3109069086965489e-1 * C110 *
                       (0. -
                        0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                   0. * C110 *
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                            0.06672455060314922 * C135 -
                        0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                            (2 * C157 + 1))) /
                  (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                   (0.06672455060314922 * C135 * (C157 + 1.) /
                        (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                    1.))) +
             0.) *
                C676) +
           ((((0. * C47 +
               0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.)) *
                  C375 +
              (C382 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) +
               C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.)) /
                  (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                   (0.06672455060314922 * C135 * (C157 + 1.) /
                        (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                    1.))) +
             C398 *
                 (C47 * 0.3109069086965489e-1 * C110 *
                      (0. - 0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                  0. * C110 *
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                           0.06672455060314922 * C135 -
                       0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                           (2 * C157 + 1))) /
                 (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                  (0.06672455060314922 * C135 * (C157 + 1.) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                   1.))) +
            0.) *
               C683) +
          ((((0. * C47 +
              0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                            1.)) *
                 C375 +
             C382 *
                 (C47 * 0.2074512376219157e-2 * (C157 + 1.) +
                  0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                  (0.06672455060314922 * C135 * (C157 + 1.) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                   1.))) +
            C398 *
                (C47 * 0.3109069086965489e-1 *
                     (C110 * (0. - 0.06672455060314922 *
                                       (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                           0.06672455060314922 * C135 -
                       0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                           (2 * C157 + 1))) +
                 0. * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1))) /
                (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                 (0.06672455060314922 * C135 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                  1.))) +
           0.) *
              C212) +
         ((((0. * C47 +
             0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.)) *
                C375 +
            C382 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                 (0.06672455060314922 * C135 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                  1.))) +
           (C398 *
                (C47 * 0.3109069086965489e-1 * C110 *
                     (0. - 0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                 0. * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1))) +
            C47 * 0.3109069086965489e-1 * C110 *
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) * 0.06672455060314922 *
                     C135 -
                 0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                     (2 * C157 + 1))) /
               (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                (0.06672455060314922 * C135 * (C157 + 1.) /
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                 1.))) +
          0.) *
             C699) +
        0.;

    const double C399 = 2.;
    const double C400 = 0.;
    const double C401 = C399;
    const double C402 = C401;
    const double C403 = C400 / C47;
    const double C404 = 0.;
    const double C405 = C403;
    const double C406 = -C403;
    const double C407 = C50 * std::pow(C67, C50 - 1) * C405;
    const double C408 = C51 * std::pow(C67, C51 - 1) * C405;
    const double C409 = C50 * std::pow(C68, C50 - 1) * C406;
    const double C410 = C51 * std::pow(C68, C51 - 1) * C406;
    const double C411 = C407 + C409;
    const double C412 = C408 + C410;
    const double C413 = std::sqrt(C73) * C411;
    const double C414 = C412;
    const double C415 = C411 / 2.;
    const double C416 = 2. * C413;
    const double C417 = 3 * std::pow(C105, 2) * C415;
    const double C418 = 0.3109069086965489e-1 * C417 + 0.;
    const double C419 = C47 * C416;
    const double C420 = C419 / 2.;
    const double C421 = C420;
    const double C422 = 2. * C421;
    const double C423 =
        C402 / (2 * std::sqrt(C61) * C124) - C422 * std::sqrt(C61) / std::pow(C124, 2);
    const double C424 = 2 * C131 * C423;
    const double C425 = 0.0667245506031492 * C424;
    const double C426 = C137 * C414;
    const double C427 = C69 * C426;
    const double C428 = 0.0337738 * C427;
    const double C429 = C144 * C414;
    const double C430 = C64 * C429;
    const double C431 = C428 / C66 - C404 * C145 / std::pow(C66, 2);
    const double C432 = C431;
    const double C433 = C430 / C65;
    const double C434 = C432 + C433;
    const double C435 = C434 / C111 - C418 * C151 / std::pow(C111, 2);
    const double C436 = -C435;
    const double C437 = std::exp(C153) * C436;
    const double C438 = C437;
    const double C439 = 0.3109069086965489e-1 * C438 + 0.;
    const double C440 = C425 / C156 - C439 * C136 / std::pow(C156, 2);
    const double C700 = (-C162 * C400) / std::pow(C47, 2);
    const double C701 = C700;
    const double C702 = -C700;
    const double C703 = C50 * (C50 - 1) * std::pow(C67, C50 - 2) * C405 * C173 +
                        C50 * std::pow(C67, C50 - 1) * C701;
    const double C704 = C51 * (C51 - 1) * std::pow(C67, C51 - 2) * C405 * C173 +
                        C51 * std::pow(C67, C51 - 1) * C701;
    const double C705 = C50 * (C50 - 1) * std::pow(C68, C50 - 2) * C406 * C174 +
                        C50 * std::pow(C68, C50 - 1) * C702;
    const double C706 = C51 * (C51 - 1) * std::pow(C68, C51 - 2) * C406 * C174 +
                        C51 * std::pow(C68, C51 - 1) * C702;
    const double C707 = C703 + C705;
    const double C708 = C704 + C706;
    const double C709 = C178 * C411 / (2 * std::sqrt(C73)) + std::sqrt(C73) * C707;
    const double C710 = C708;
    const double C711 = C707 / 2.;
    const double C712 = 2. * C709;
    const double C713 = 6 * C105 * C415 * C207 + 3 * std::pow(C105, 2) * C711;
    const double C714 = 0.3109069086965489e-1 * C713 + 0.;
    const double C715 = C416 * C162 + C47 * C712;
    const double C716 = C715 / 2.;
    const double C717 = C716;
    const double C718 = 2. * C717;
    const double C719 = ((-C402 * 2 * std::sqrt(C61)) / std::pow(2 * std::sqrt(C61) * C124, 2) -
                         (-C422 * std::sqrt(C61) * 2 * C124) / std::pow(C124, 4)) *
                            C226 -
                        C718 * std::sqrt(C61) / std::pow(C124, 2);
    const double C720 = 2 * C423 * C233 + 2 * C131 * C719;
    const double C721 = 0.0667245506031492 * C720;
    const double C722 = C414 * C239 + C137 * C710;
    const double C723 = C426 * C175 + C69 * C722;
    const double C724 = 0.0337738 * C723;
    const double C725 = C414 * C246 + C144 * C710;
    const double C726 = C429 * C171 + C64 * C725;
    const double C727 =
        (C724 / C66 +
         ((-C428) / std::pow(C66, 2) - (-C404 * C145 * 2 * C66) / std::pow(C66, 4)) * C172) -
        C247 * C404 / std::pow(C66, 2);
    const double C728 = C727;
    const double C729 = C726 / C65;
    const double C730 = C728 + C729;
    const double C731 =
        ((C730 / C111 +
          ((-C434) / std::pow(C111, 2) - (-C418 * C151 * 2 * C111) / std::pow(C111, 4)) * C213) -
         C714 * C151 / std::pow(C111, 2)) -
        C253 * C418 / std::pow(C111, 2);
    const double C732 = -C731;
    const double C733 = std::exp(C153) * C436 * C255 + std::exp(C153) * C732;
    const double C734 = C733;
    const double C735 = 0.3109069086965489e-1 * C734 + 0.;
    const double C736 =
        ((C721 / C156 +
          ((-C425) / std::pow(C156, 2) - (-C439 * C136 * 2 * C156) / std::pow(C156, 4)) * C258) -
         C735 * C136 / std::pow(C156, 2)) -
        C238 * C439 / std::pow(C156, 2);
    matrix.upper[3] =
        (((((((((((0. * C47 +
                   0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                 1.)) *
                      C417 +
                  C424 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.))) +
                 C440 *
                     (C47 * 0.3109069086965489e-1 * C110 *
                          (0. -
                           0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                      0. * C110 *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1))) /
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.))) +
                ((((1. * C434 + (0. * C47 + 0.3109069086965489e-1 *
                                                std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                                             (0.3109069086965489e-1 *
                                                              ((C157 + std::pow(C157, 2)) + 1.)) +
                                                         1.)) *
                                    C417) +
                   C424 *
                       (0. * C47 +
                        0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.)) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                        (0.06672455060314922 * C135 * (C157 + 1.) /
                             (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                         1.))) +
                  C440 *
                      (C47 * 0.3109069086965489e-1 * C110 *
                           (0. -
                            0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                       0.3109069086965489e-1 * C110 *
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                0.06672455060314922 * C135 -
                            0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                                (2 * C157 + 1))) /
                      (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.))) +
                 0.) *
                    C162) +
               ((((1. * C47 + (0. * C47 + 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                                            (0.3109069086965489e-1 *
                                                             ((C157 + std::pow(C157, 2)) + 1.)) +
                                                        1.)) *
                                  C417) +
                  C424 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.))) +
                 C440 *
                     (C47 * 0.3109069086965489e-1 * C110 *
                          (0. -
                           0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                      0. * C110 *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1))) /
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.))) +
                0.) *
                   C730) +
              ((((C47 * 0.2074512376219157e-2 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.)) +
                  0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                    (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                1.)) *
                     C417 +
                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.) *
                      C424 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) -
                  C424 * C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.) *
                      0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                      0.06672455060314922 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.))) /
                     std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                  (0.06672455060314922 * C135 * (C157 + 1.) /
                                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                   1.),
                              2)) +
                (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                     (0.06672455060314922 * C135 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      1.) *
                     C440 *
                     (C47 * 0.3109069086965489e-1 * C110 *
                          (0.2074512376219157e-2 * ((C157 + std::pow(C157, 2)) + 1.) -
                           0.06672455060314922 *
                               (0. * C135 * (C157 + 1.) +
                                (C157 + 1.) * 0.3109069086965489e-1 * (2 * C157 + 1))) +
                      0. * C110 *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1))) -
                 C440 * C47 * 0.3109069086965489e-1 * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1)) *
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                          0.06672455060314922 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      0. * ((C157 + std::pow(C157, 2)) + 1.) *
                          (0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.))) /
                    std::pow(
                        std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                            (0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.),
                        2)) +
               0.) *
                  C237) +
             ((((C47 * 0.3109069086965489e-1 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1)) /
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.)) +
                 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                               1.)) *
                    C417 +
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                     (0.06672455060314922 * C135 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      1.) *
                     C424 *
                     (C47 * 0.2074512376219157e-2 * C110 +
                      0. * C110 * 0.06672455060314922 * (C157 + 1.)) -
                 C424 * C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.) *
                     0.3109069086965489e-1 *
                     (((C157 + std::pow(C157, 2)) + 1.) *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1)) /
                          std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) +
                      (2 * C157 + 1) *
                          (0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.))) /
                    std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                 (0.06672455060314922 * C135 * (C157 + 1.) /
                                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                  1.),
                             2)) +
               (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                    (0.06672455060314922 * C135 * (C157 + 1.) /
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                     1.) *
                    C440 *
                    (C47 * 0.3109069086965489e-1 * C110 *
                         (0.3109069086965489e-1 * (2 * C157 + 1) * 0.06672455060314922 * C135 -
                          0.06672455060314922 * (0.6218138173930979e-1 * C135 * (C157 + 1.) +
                                                 C135 * 0.3109069086965489e-1 * (2 * C157 + 1))) +
                     0. * C110 *
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                              0.06672455060314922 * C135 -
                          0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                              (2 * C157 + 1))) -
                C440 * C47 * 0.3109069086965489e-1 * C110 *
                    (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                         0.06672455060314922 * C135 -
                     0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                         (2 * C157 + 1)) *
                    (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                              0.06672455060314922 * C135 -
                          0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                              (2 * C157 + 1)) /
                         std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) +
                     0.6218138173930979e-1 * (2 * C157 + 1) * 0.3109069086965489e-1 *
                         ((C157 + std::pow(C157, 2)) + 1.) *
                         (0.06672455060314922 * C135 * (C157 + 1.) /
                              (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                          1.))) /
                   std::pow(std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                                (0.06672455060314922 * C135 * (C157 + 1.) /
                                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                 1.),
                            2)) +
              0.) *
                 C259) +
            ((((C47 * 0.3109069086965489e-1 *
                    std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.) +
                (0. * C47 +
                 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                               1.)) *
                    C417) +
               C424 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                    (0.06672455060314922 * C135 * (C157 + 1.) /
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                     1.))) +
              C440 *
                  (C47 * 0.3109069086965489e-1 * C110 *
                       (0. -
                        0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                   0. * C110 *
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                            0.06672455060314922 * C135 -
                        0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                            (2 * C157 + 1))) /
                  (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                   (0.06672455060314922 * C135 * (C157 + 1.) /
                        (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                    1.))) +
             0.) *
                C713) +
           ((((0. * C47 +
               0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.)) *
                  C417 +
              (C424 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) +
               C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.)) /
                  (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                   (0.06672455060314922 * C135 * (C157 + 1.) /
                        (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                    1.))) +
             C440 *
                 (C47 * 0.3109069086965489e-1 * C110 *
                      (0. - 0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                  0. * C110 *
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                           0.06672455060314922 * C135 -
                       0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                           (2 * C157 + 1))) /
                 (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                  (0.06672455060314922 * C135 * (C157 + 1.) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                   1.))) +
            0.) *
               C720) +
          ((((0. * C47 +
              0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                            1.)) *
                 C417 +
             C424 *
                 (C47 * 0.2074512376219157e-2 * (C157 + 1.) +
                  0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                  (0.06672455060314922 * C135 * (C157 + 1.) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                   1.))) +
            C440 *
                (C47 * 0.3109069086965489e-1 *
                     (C110 * (0. - 0.06672455060314922 *
                                       (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                           0.06672455060314922 * C135 -
                       0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                           (2 * C157 + 1))) +
                 0. * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1))) /
                (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                 (0.06672455060314922 * C135 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                  1.))) +
           0.) *
              C212) +
         ((((0. * C47 +
             0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.)) *
                C417 +
            C424 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                 (0.06672455060314922 * C135 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                  1.))) +
           (C440 *
                (C47 * 0.3109069086965489e-1 * C110 *
                     (0. - 0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                 0. * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1))) +
            C47 * 0.3109069086965489e-1 * C110 *
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) * 0.06672455060314922 *
                     C135 -
                 0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                     (2 * C157 + 1))) /
               (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                (0.06672455060314922 * C135 * (C157 + 1.) /
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                 1.))) +
          0.) *
             C736) +
        0.;

    const double C441 = 0.;
    const double C442 = 1;
    const double C443 = C441 / C47;
    const double C444 = 0.;
    const double C445 = C443;
    const double C446 = -C443;
    const double C447 = C50 * std::pow(C67, C50 - 1) * C445;
    const double C448 = C51 * std::pow(C67, C51 - 1) * C445;
    const double C449 = C50 * std::pow(C68, C50 - 1) * C446;
    const double C450 = C51 * std::pow(C68, C51 - 1) * C446;
    const double C451 = C447 + C449;
    const double C452 = C448 + C450;
    const double C453 = std::sqrt(C73) * C451;
    const double C454 = C452;
    const double C455 = C451 / 2.;
    const double C456 = 2. * C453;
    const double C457 = 3 * std::pow(C105, 2) * C455;
    const double C458 = 0.3109069086965489e-1 * C457 + 0.;
    const double C459 = C47 * C456;
    const double C460 = C459 / 2.;
    const double C461 = C460;
    const double C462 = 2. * C461;
    const double C463 =
        C442 / (2 * std::sqrt(C61) * C124) - C462 * std::sqrt(C61) / std::pow(C124, 2);
    const double C464 = 2 * C131 * C463;
    const double C465 = 0.0667245506031492 * C464;
    const double C466 = C137 * C454;
    const double C467 = C69 * C466;
    const double C468 = 0.0337738 * C467;
    const double C469 = C144 * C454;
    const double C470 = C64 * C469;
    const double C471 = C468 / C66 - C444 * C145 / std::pow(C66, 2);
    const double C472 = C471;
    const double C473 = C470 / C65;
    const double C474 = C472 + C473;
    const double C475 = C474 / C111 - C458 * C151 / std::pow(C111, 2);
    const double C476 = -C475;
    const double C477 = std::exp(C153) * C476;
    const double C478 = C477;
    const double C479 = 0.3109069086965489e-1 * C478 + 0.;
    const double C480 = C465 / C156 - C479 * C136 / std::pow(C156, 2);
    const double C737 = (-C162 * C441) / std::pow(C47, 2);
    const double C738 = C737;
    const double C739 = -C737;
    const double C740 = C50 * (C50 - 1) * std::pow(C67, C50 - 2) * C445 * C173 +
                        C50 * std::pow(C67, C50 - 1) * C738;
    const double C741 = C51 * (C51 - 1) * std::pow(C67, C51 - 2) * C445 * C173 +
                        C51 * std::pow(C67, C51 - 1) * C738;
    const double C742 = C50 * (C50 - 1) * std::pow(C68, C50 - 2) * C446 * C174 +
                        C50 * std::pow(C68, C50 - 1) * C739;
    const double C743 = C51 * (C51 - 1) * std::pow(C68, C51 - 2) * C446 * C174 +
                        C51 * std::pow(C68, C51 - 1) * C739;
    const double C744 = C740 + C742;
    const double C745 = C741 + C743;
    const double C746 = C178 * C451 / (2 * std::sqrt(C73)) + std::sqrt(C73) * C744;
    const double C747 = C745;
    const double C748 = C744 / 2.;
    const double C749 = 2. * C746;
    const double C750 = 6 * C105 * C455 * C207 + 3 * std::pow(C105, 2) * C748;
    const double C751 = 0.3109069086965489e-1 * C750 + 0.;
    const double C752 = C456 * C162 + C47 * C749;
    const double C753 = C752 / 2.;
    const double C754 = C753;
    const double C755 = 2. * C754;
    const double C756 = ((-C442 * 2 * std::sqrt(C61)) / std::pow(2 * std::sqrt(C61) * C124, 2) -
                         (-C462 * std::sqrt(C61) * 2 * C124) / std::pow(C124, 4)) *
                            C226 -
                        C755 * std::sqrt(C61) / std::pow(C124, 2);
    const double C757 = 2 * C463 * C233 + 2 * C131 * C756;
    const double C758 = 0.0667245506031492 * C757;
    const double C759 = C454 * C239 + C137 * C747;
    const double C760 = C466 * C175 + C69 * C759;
    const double C761 = 0.0337738 * C760;
    const double C762 = C454 * C246 + C144 * C747;
    const double C763 = C469 * C171 + C64 * C762;
    const double C764 =
        (C761 / C66 +
         ((-C468) / std::pow(C66, 2) - (-C444 * C145 * 2 * C66) / std::pow(C66, 4)) * C172) -
        C247 * C444 / std::pow(C66, 2);
    const double C765 = C764;
    const double C766 = C763 / C65;
    const double C767 = C765 + C766;
    const double C768 =
        ((C767 / C111 +
          ((-C474) / std::pow(C111, 2) - (-C458 * C151 * 2 * C111) / std::pow(C111, 4)) * C213) -
         C751 * C151 / std::pow(C111, 2)) -
        C253 * C458 / std::pow(C111, 2);
    const double C769 = -C768;
    const double C770 = std::exp(C153) * C476 * C255 + std::exp(C153) * C769;
    const double C771 = C770;
    const double C772 = 0.3109069086965489e-1 * C771 + 0.;
    const double C773 =
        ((C758 / C156 +
          ((-C465) / std::pow(C156, 2) - (-C479 * C136 * 2 * C156) / std::pow(C156, 4)) * C258) -
         C772 * C136 / std::pow(C156, 2)) -
        C238 * C479 / std::pow(C156, 2);
    matrix.upper[4] =
        (((((((((((0. * C47 +
                   0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                 1.)) *
                      C457 +
                  C464 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.))) +
                 C480 *
                     (C47 * 0.3109069086965489e-1 * C110 *
                          (0. -
                           0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                      0. * C110 *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1))) /
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.))) +
                ((((1. * C474 + (0. * C47 + 0.3109069086965489e-1 *
                                                std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                                             (0.3109069086965489e-1 *
                                                              ((C157 + std::pow(C157, 2)) + 1.)) +
                                                         1.)) *
                                    C457) +
                   C464 *
                       (0. * C47 +
                        0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.)) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                        (0.06672455060314922 * C135 * (C157 + 1.) /
                             (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                         1.))) +
                  C480 *
                      (C47 * 0.3109069086965489e-1 * C110 *
                           (0. -
                            0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                       0.3109069086965489e-1 * C110 *
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                0.06672455060314922 * C135 -
                            0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                                (2 * C157 + 1))) /
                      (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.))) +
                 0.) *
                    C162) +
               ((((1. * C47 + (0. * C47 + 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                                            (0.3109069086965489e-1 *
                                                             ((C157 + std::pow(C157, 2)) + 1.)) +
                                                        1.)) *
                                  C457) +
                  C464 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.))) +
                 C480 *
                     (C47 * 0.3109069086965489e-1 * C110 *
                          (0. -
                           0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                      0. * C110 *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1))) /
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.))) +
                0.) *
                   C767) +
              ((((C47 * 0.2074512376219157e-2 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.)) +
                  0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                    (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                1.)) *
                     C457 +
                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.) *
                      C464 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) -
                  C464 * C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.) *
                      0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                      0.06672455060314922 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.))) /
                     std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                  (0.06672455060314922 * C135 * (C157 + 1.) /
                                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                   1.),
                              2)) +
                (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                     (0.06672455060314922 * C135 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      1.) *
                     C480 *
                     (C47 * 0.3109069086965489e-1 * C110 *
                          (0.2074512376219157e-2 * ((C157 + std::pow(C157, 2)) + 1.) -
                           0.06672455060314922 *
                               (0. * C135 * (C157 + 1.) +
                                (C157 + 1.) * 0.3109069086965489e-1 * (2 * C157 + 1))) +
                      0. * C110 *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1))) -
                 C480 * C47 * 0.3109069086965489e-1 * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1)) *
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                          0.06672455060314922 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      0. * ((C157 + std::pow(C157, 2)) + 1.) *
                          (0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.))) /
                    std::pow(
                        std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                            (0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.),
                        2)) +
               0.) *
                  C237) +
             ((((C47 * 0.3109069086965489e-1 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1)) /
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.)) +
                 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                               1.)) *
                    C457 +
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                     (0.06672455060314922 * C135 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      1.) *
                     C464 *
                     (C47 * 0.2074512376219157e-2 * C110 +
                      0. * C110 * 0.06672455060314922 * (C157 + 1.)) -
                 C464 * C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.) *
                     0.3109069086965489e-1 *
                     (((C157 + std::pow(C157, 2)) + 1.) *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1)) /
                          std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) +
                      (2 * C157 + 1) *
                          (0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.))) /
                    std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                 (0.06672455060314922 * C135 * (C157 + 1.) /
                                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                  1.),
                             2)) +
               (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                    (0.06672455060314922 * C135 * (C157 + 1.) /
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                     1.) *
                    C480 *
                    (C47 * 0.3109069086965489e-1 * C110 *
                         (0.3109069086965489e-1 * (2 * C157 + 1) * 0.06672455060314922 * C135 -
                          0.06672455060314922 * (0.6218138173930979e-1 * C135 * (C157 + 1.) +
                                                 C135 * 0.3109069086965489e-1 * (2 * C157 + 1))) +
                     0. * C110 *
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                              0.06672455060314922 * C135 -
                          0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                              (2 * C157 + 1))) -
                C480 * C47 * 0.3109069086965489e-1 * C110 *
                    (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                         0.06672455060314922 * C135 -
                     0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                         (2 * C157 + 1)) *
                    (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                              0.06672455060314922 * C135 -
                          0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                              (2 * C157 + 1)) /
                         std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) +
                     0.6218138173930979e-1 * (2 * C157 + 1) * 0.3109069086965489e-1 *
                         ((C157 + std::pow(C157, 2)) + 1.) *
                         (0.06672455060314922 * C135 * (C157 + 1.) /
                              (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                          1.))) /
                   std::pow(std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                                (0.06672455060314922 * C135 * (C157 + 1.) /
                                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                 1.),
                            2)) +
              0.) *
                 C259) +
            ((((C47 * 0.3109069086965489e-1 *
                    std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.) +
                (0. * C47 +
                 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                               1.)) *
                    C457) +
               C464 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                    (0.06672455060314922 * C135 * (C157 + 1.) /
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                     1.))) +
              C480 *
                  (C47 * 0.3109069086965489e-1 * C110 *
                       (0. -
                        0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                   0. * C110 *
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                            0.06672455060314922 * C135 -
                        0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                            (2 * C157 + 1))) /
                  (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                   (0.06672455060314922 * C135 * (C157 + 1.) /
                        (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                    1.))) +
             0.) *
                C750) +
           ((((0. * C47 +
               0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.)) *
                  C457 +
              (C464 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) +
               C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.)) /
                  (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                   (0.06672455060314922 * C135 * (C157 + 1.) /
                        (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                    1.))) +
             C480 *
                 (C47 * 0.3109069086965489e-1 * C110 *
                      (0. - 0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                  0. * C110 *
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                           0.06672455060314922 * C135 -
                       0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                           (2 * C157 + 1))) /
                 (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                  (0.06672455060314922 * C135 * (C157 + 1.) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                   1.))) +
            0.) *
               C757) +
          ((((0. * C47 +
              0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                            1.)) *
                 C457 +
             C464 *
                 (C47 * 0.2074512376219157e-2 * (C157 + 1.) +
                  0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                  (0.06672455060314922 * C135 * (C157 + 1.) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                   1.))) +
            C480 *
                (C47 * 0.3109069086965489e-1 *
                     (C110 * (0. - 0.06672455060314922 *
                                       (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                           0.06672455060314922 * C135 -
                       0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                           (2 * C157 + 1))) +
                 0. * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1))) /
                (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                 (0.06672455060314922 * C135 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                  1.))) +
           0.) *
              C212) +
         ((((0. * C47 +
             0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.)) *
                C457 +
            C464 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                 (0.06672455060314922 * C135 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                  1.))) +
           (C480 *
                (C47 * 0.3109069086965489e-1 * C110 *
                     (0. - 0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                 0. * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1))) +
            C47 * 0.3109069086965489e-1 * C110 *
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) * 0.06672455060314922 *
                     C135 -
                 0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                     (2 * C157 + 1))) /
               (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                (0.06672455060314922 * C135 * (C157 + 1.) /
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                 1.))) +
          0.) *
             C773) +
        0.;

    const double C774 = 0.;
    const double C775 =
        ((-C261) / std::pow(C47, 2) - (-C260 * C48 * 2 * C47) / std::pow(C47, 4)) * C260 -
        C261 * C260 / std::pow(C47, 2);
    const double C776 =
        (C774 / C47 +
         ((-C262) / std::pow(C47, 2) - (-C260 * C53 * 2 * C47) / std::pow(C47, 4)) * C260) -
        C262 * C260 / std::pow(C47, 2);
    const double C777 = 12 * std::pow(C57, 2) * std::pow(C265, 2) + 4 * std::pow(C57, 3) * C775;
    const double C778 = C776;
    const double C779 = -C776;
    const double C780 = -C777;
    const double C781 = (-C267 * -3. * C267 * 2 * C60) / std::pow(C60, 4);
    const double C782 = C49 * (C49 - 1) * std::pow(C59, C49 - 2) * std::pow(C266, 2);
    const double C783 = C782 / Pi;
    const double C784 = C50 * (C50 - 1) * std::pow(C67, C50 - 2) * std::pow(C271, 2) +
                        C50 * std::pow(C67, C50 - 1) * C778;
    const double C785 = C51 * (C51 - 1) * std::pow(C67, C51 - 2) * std::pow(C271, 2) +
                        C51 * std::pow(C67, C51 - 1) * C778;
    const double C786 = C50 * (C50 - 1) * std::pow(C68, C50 - 2) * std::pow(C272, 2) +
                        C50 * std::pow(C68, C50 - 1) * C779;
    const double C787 = C51 * (C51 - 1) * std::pow(C68, C51 - 2) * std::pow(C272, 2) +
                        C51 * std::pow(C68, C51 - 1) * C779;
    const double C788 = C49 * (C49 - 1) * std::pow(C70, C49 - 2) * std::pow(C274, 2) +
                        C49 * std::pow(C70, C49 - 1) * C781;
    const double C789 = C50 * (C50 - 1) * std::pow(C70, C50 - 2) * std::pow(C274, 2) +
                        C50 * std::pow(C70, C50 - 1) * C781;
    const double C790 = 0.11125 * C788;
    const double C791 = 0.20548 * C788;
    const double C792 = 0.2137 * C788;
    const double C793 = 0.49294 * C789;
    const double C794 = 0.49671 * C789;
    const double C795 = 0.62517 * C789;
    const double C796 =
        0.88026 * C781 / (2 * std::sqrt(C70)) -
        0.88026 * std::pow(C274, 2) / (std::sqrt(C70) * std::pow(2 * std::sqrt(C70), 2));
    const double C797 =
        1.6382 * C781 / (2 * std::sqrt(C70)) -
        1.6382 * std::pow(C274, 2) / (std::sqrt(C70) * std::pow(2 * std::sqrt(C70), 2));
    const double C798 =
        3.3662 * C781 / (2 * std::sqrt(C70)) -
        3.3662 * std::pow(C274, 2) / (std::sqrt(C70) * std::pow(2 * std::sqrt(C70), 2));
    const double C799 = 3.5876 * C788;
    const double C800 = 3.6231 * C788;
    const double C801 = 6.1977 * C788;
    const double C802 = C784 + C786;
    const double C803 = C785 + C787;
    const double C804 =
        7.5957 * C788 / (2 * std::sqrt(C79)) -
        7.5957 * std::pow(C281, 2) / (std::sqrt(C79) * std::pow(2 * std::sqrt(C79), 2));
    const double C805 =
        10.357 * C788 / (2 * std::sqrt(C79)) -
        10.357 * std::pow(C281, 2) / (std::sqrt(C79) * std::pow(2 * std::sqrt(C79), 2));
    const double C806 =
        0.141189e2 * C788 / (2 * std::sqrt(C79)) -
        0.141189e2 * std::pow(C281, 2) / (std::sqrt(C79) * std::pow(2 * std::sqrt(C79), 2));
    const double C807 = ((C783 * C93 + C295 * C276) / (2 * std::sqrt(C73)) +
                         (C295 / (2 * std::sqrt(C73)) -
                          C276 * C93 / (std::sqrt(C73) * std::pow(2 * std::sqrt(C73), 2))) *
                             C276) +
                        std::sqrt(C73) * C802;
    const double C808 = C790;
    const double C809 = C791;
    const double C810 = C792;
    const double C811 = C803;
    const double C812 = C802 / 2.;
    const double C813 = 2. * C807;
    const double C814 = C804 + C799;
    const double C815 = C805 + C800;
    const double C816 = C806 + C801;
    const double C817 = 6 * C105 * std::pow(C305, 2) + 3 * std::pow(C105, 2) * C812;
    const double C818 = 0.3109069086965489e-1 * C817 + 0.;
    const double C819 = (C260 * C306 + C306 * C260) + C47 * C813;
    const double C820 = C814 + C797;
    const double C821 = C815 + C796;
    const double C822 = C816 + C798;
    const double C823 = C820 + C793;
    const double C824 = C821 + C794;
    const double C825 = C822 + C795;
    const double C826 = C819 / 2.;
    const double C827 = 0.0310907 * C825;
    const double C828 = 0.0337738 * C824;
    const double C829 = 0.0621814 * C823;
    const double C830 = C826;
    const double C831 = 2. * C830;
    const double C832 = 2 * C120 * std::pow(C320, 2) / std::pow(C120, 4) - C827 / std::pow(C120, 2);
    const double C833 = 2 * C121 * std::pow(C321, 2) / std::pow(C121, 4) - C828 / std::pow(C121, 2);
    const double C834 = 2 * C122 * std::pow(C322, 2) / std::pow(C122, 4) - C829 / std::pow(C122, 2);
    const double C835 = C832;
    const double C836 = C833;
    const double C837 = C834;
    const double C838 = C324 * C324 * std::sqrt(C61) * 2 * C124 / std::pow(C124, 4) -
                        C831 * std::sqrt(C61) / std::pow(C124, 2);
    const double C839 = C835 / C128 - std::pow(C328, 2) / std::pow(C128, 2);
    const double C840 = C836 / C129 - std::pow(C329, 2) / std::pow(C129, 2);
    const double C841 = C837 / C130 - std::pow(C330, 2) / std::pow(C130, 2);
    const double C842 = 2 * std::pow(C331, 2) + 2 * C131 * C838;
    const double C843 = 0.0667245506031492 * C842;
    const double C844 = ((C301 * C333 + C133 * C808) + C333 * C301) + C101 * C840;
    const double C845 = ((C302 * C332 + C132 * C809) + C332 * C302) + C102 * C839;
    const double C846 = ((C303 * C334 + C134 * C810) + C334 * C303) + C103 * C841;
    const double C847 = 0.0310907 * C845;
    const double C848 = 0.0621814 * C846;
    const double C849 = ((C337 * C304 + C104 * C844) + C304 * C337) + C137 * C811;
    const double C850 = ((C342 * C273 + C69 * C849) + C273 * C342) + C142 * C780;
    const double C851 = C848 - C847;
    const double C852 = 0.0337738 * C850;
    const double C853 = ((C344 * C304 + C104 * C851) + C304 * C344) + C144 * C811;
    const double C854 = ((C269 * C346 + C146 * C777) + C346 * C269) + C64 * C853;
    const double C855 =
        (C852 / C66 +
         ((-C345) / std::pow(C66, 2) - (-C270 * C145 * 2 * C66) / std::pow(C66, 4)) * C270) -
        C345 * C270 / std::pow(C66, 2);
    const double C856 = C855 - C848;
    const double C857 = C854 / C65;
    const double C858 = C856 + C857;
    const double C859 =
        ((C858 / C111 +
          ((-C351) / std::pow(C111, 2) - (-C311 * C151 * 2 * C111) / std::pow(C111, 4)) * C311) -
         C818 * C151 / std::pow(C111, 2)) -
        C351 * C311 / std::pow(C111, 2);
    const double C860 = -C859;
    const double C861 = std::exp(C153) * std::pow(C353, 2) + std::exp(C153) * C860;
    const double C862 = C861;
    const double C863 = 0.3109069086965489e-1 * C862 + 0.;
    const double C864 =
        ((C843 / C156 +
          ((-C336) / std::pow(C156, 2) - (-C356 * C136 * 2 * C156) / std::pow(C156, 4)) * C356) -
         C863 * C136 / std::pow(C156, 2)) -
        C336 * C356 / std::pow(C156, 2);
    matrix.upper[5] =
        ((((((((((((0. * C260 + (0. * C47 + 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                                              (0.3109069086965489e-1 *
                                                               ((C157 + std::pow(C157, 2)) + 1.)) +
                                                          1.)) *
                                    C310) +
                   C335 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                        (0.06672455060314922 * C135 * (C157 + 1.) /
                             (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                         1.))) +
                  C357 *
                      (C47 * 0.3109069086965489e-1 * C110 *
                           (0. -
                            0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                       0. * C110 *
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                0.06672455060314922 * C135 -
                            0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                                (2 * C157 + 1))) /
                      (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.))) +
                 (((((0. * C260 + 1. * C351) +
                     (0. * C47 +
                      0.3109069086965489e-1 *
                          std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                   1.)) *
                         C310) +
                    C335 *
                        (0. * C47 +
                         0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.)) /
                        (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                         (0.06672455060314922 * C135 * (C157 + 1.) /
                              (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                          1.))) +
                   C357 *
                       (C47 * 0.3109069086965489e-1 * C110 *
                            (0. - 0.06672455060314922 *
                                      (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                        0.3109069086965489e-1 * C110 *
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                 0.06672455060314922 * C135 -
                             0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                                 (2 * C157 + 1))) /
                       (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                        (0.06672455060314922 * C135 * (C157 + 1.) /
                             (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                         1.))) +
                  0.) *
                     C260) +
                ((((1. * C260 + (0. * C47 + 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                                              (0.3109069086965489e-1 *
                                                               ((C157 + std::pow(C157, 2)) + 1.)) +
                                                          1.)) *
                                    C310) +
                   C335 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                        (0.06672455060314922 * C135 * (C157 + 1.) /
                             (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                         1.))) +
                  C357 *
                      (C47 * 0.3109069086965489e-1 * C110 *
                           (0. -
                            0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                       0. * C110 *
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                0.06672455060314922 * C135 -
                            0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                                (2 * C157 + 1))) /
                      (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.))) +
                 0.) *
                    C351) +
               (((((0.3109069086965489e-1 *
                        std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                 1.) +
                    0.) *
                       C260 +
                   (0. * C47 +
                    0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                  1.)) *
                       C310) +
                  C335 *
                      (C47 * 0.2074512376219157e-2 * (C157 + 1.) +
                       0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.))) +
                 C357 *
                     (C47 * 0.3109069086965489e-1 *
                          (C110 * (0. - 0.06672455060314922 *
                                            (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                0.06672455060314922 * C135 -
                            0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                                (2 * C157 + 1))) +
                      0. * C110 *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1))) /
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.))) +
                0.) *
                   C310) +
              (((((0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                        (0.06672455060314922 * C135 * (C157 + 1.) /
                             (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                         1.)) +
                   0.) *
                      C260 +
                  (C47 * 0.2074512376219157e-2 * (C157 + 1.) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                        (0.06672455060314922 * C135 * (C157 + 1.) /
                             (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                         1.)) +
                   0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                 1.)) *
                      C310) +
                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.) *
                      C335 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) -
                  C335 * C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.) *
                      0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                      0.06672455060314922 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.))) /
                     std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                  (0.06672455060314922 * C135 * (C157 + 1.) /
                                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                   1.),
                              2)) +
                (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                     (0.06672455060314922 * C135 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      1.) *
                     C357 *
                     (C47 * 0.3109069086965489e-1 * C110 *
                          (0.2074512376219157e-2 * ((C157 + std::pow(C157, 2)) + 1.) -
                           0.06672455060314922 *
                               (0. * C135 * (C157 + 1.) +
                                (C157 + 1.) * 0.3109069086965489e-1 * (2 * C157 + 1))) +
                      0. * C110 *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1))) -
                 C357 * C47 * 0.3109069086965489e-1 * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1)) *
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                          0.06672455060314922 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      0. * ((C157 + std::pow(C157, 2)) + 1.) *
                          (0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.))) /
                    std::pow(
                        std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                            (0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.),
                        2)) +
               0.) *
                  C335) +
             (((((0.3109069086965489e-1 * C110 *
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                           0.06672455060314922 * C135 -
                       0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                           (2 * C157 + 1)) /
                      (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.)) +
                  0.) *
                     C260 +
                 (C47 * 0.3109069086965489e-1 *
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                           0.06672455060314922 * C135 -
                       0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                           (2 * C157 + 1)) /
                      (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.)) +
                  0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                    (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                1.)) *
                     C310) +
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                     (0.06672455060314922 * C135 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      1.) *
                     C335 *
                     (C47 * 0.2074512376219157e-2 * C110 +
                      0. * C110 * 0.06672455060314922 * (C157 + 1.)) -
                 C335 * C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.) *
                     0.3109069086965489e-1 *
                     (((C157 + std::pow(C157, 2)) + 1.) *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1)) /
                          std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) +
                      (2 * C157 + 1) *
                          (0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.))) /
                    std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                 (0.06672455060314922 * C135 * (C157 + 1.) /
                                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                  1.),
                             2)) +
               (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                    (0.06672455060314922 * C135 * (C157 + 1.) /
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                     1.) *
                    C357 *
                    (C47 * 0.3109069086965489e-1 * C110 *
                         (0.3109069086965489e-1 * (2 * C157 + 1) * 0.06672455060314922 * C135 -
                          0.06672455060314922 * (0.6218138173930979e-1 * C135 * (C157 + 1.) +
                                                 C135 * 0.3109069086965489e-1 * (2 * C157 + 1))) +
                     0. * C110 *
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                              0.06672455060314922 * C135 -
                          0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                              (2 * C157 + 1))) -
                C357 * C47 * 0.3109069086965489e-1 * C110 *
                    (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                         0.06672455060314922 * C135 -
                     0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                         (2 * C157 + 1)) *
                    (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                              0.06672455060314922 * C135 -
                          0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                              (2 * C157 + 1)) /
                         std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) +
                     0.6218138173930979e-1 * (2 * C157 + 1) * 0.3109069086965489e-1 *
                         ((C157 + std::pow(C157, 2)) + 1.) *
                         (0.06672455060314922 * C135 * (C157 + 1.) /
                              (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                          1.))) /
                   std::pow(std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                                (0.06672455060314922 * C135 * (C157 + 1.) /
                                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                 1.),
                            2)) +
              0.) *
                 C357) +
            (((((0. * C260 + 1. * C47) +
                (0. * C47 +
                 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                               1.)) *
                    C310) +
               C335 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                    (0.06672455060314922 * C135 * (C157 + 1.) /
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                     1.))) +
              C357 *
                  (C47 * 0.3109069086965489e-1 * C110 *
                       (0. -
                        0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                   0. * C110 *
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                            0.06672455060314922 * C135 -
                        0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                            (2 * C157 + 1))) /
                  (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                   (0.06672455060314922 * C135 * (C157 + 1.) /
                        (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                    1.))) +
             0.) *
                C858) +
           ((((0. * C260 +
               (C47 * 0.3109069086965489e-1 *
                    std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.) +
                (0. * C47 +
                 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                               1.)) *
                    C310)) +
              C335 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                  (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                   (0.06672455060314922 * C135 * (C157 + 1.) /
                        (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                    1.))) +
             C357 *
                 (C47 * 0.3109069086965489e-1 * C110 *
                      (0. - 0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                  0. * C110 *
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                           0.06672455060314922 * C135 -
                       0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                           (2 * C157 + 1))) /
                 (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                  (0.06672455060314922 * C135 * (C157 + 1.) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                   1.))) +
            0.) *
               C817) +
          ((((0. * C260 + (0. * C47 + 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                                        (0.3109069086965489e-1 *
                                                         ((C157 + std::pow(C157, 2)) + 1.)) +
                                                    1.)) *
                              C310) +
             (C335 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) +
              C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.)) /
                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                  (0.06672455060314922 * C135 * (C157 + 1.) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                   1.))) +
            C357 *
                (C47 * 0.3109069086965489e-1 * C110 *
                     (0. - 0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                 0. * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1))) /
                (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                 (0.06672455060314922 * C135 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                  1.))) +
           0.) *
              C842) +
         ((((0. * C260 + (0. * C47 + 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                                       (0.3109069086965489e-1 *
                                                        ((C157 + std::pow(C157, 2)) + 1.)) +
                                                   1.)) *
                             C310) +
            C335 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                 (0.06672455060314922 * C135 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                  1.))) +
           (C357 *
                (C47 * 0.3109069086965489e-1 * C110 *
                     (0. - 0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                 0. * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1))) +
            C47 * 0.3109069086965489e-1 * C110 *
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) * 0.06672455060314922 *
                     C135 -
                 0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                     (2 * C157 + 1))) /
               (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                (0.06672455060314922 * C135 * (C157 + 1.) /
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                 1.))) +
          0.) *
             C864) +
        0.;

    const double C865 = (-C260 * C358) / std::pow(C47, 2);
    const double C866 = C865;
    const double C867 = -C865;
    const double C868 = C50 * (C50 - 1) * std::pow(C67, C50 - 2) * C363 * C271 +
                        C50 * std::pow(C67, C50 - 1) * C866;
    const double C869 = C51 * (C51 - 1) * std::pow(C67, C51 - 2) * C363 * C271 +
                        C51 * std::pow(C67, C51 - 1) * C866;
    const double C870 = C50 * (C50 - 1) * std::pow(C68, C50 - 2) * C364 * C272 +
                        C50 * std::pow(C68, C50 - 1) * C867;
    const double C871 = C51 * (C51 - 1) * std::pow(C68, C51 - 2) * C364 * C272 +
                        C51 * std::pow(C68, C51 - 1) * C867;
    const double C872 = C868 + C870;
    const double C873 = C869 + C871;
    const double C874 = C276 * C369 / (2 * std::sqrt(C73)) + std::sqrt(C73) * C872;
    const double C875 = C873;
    const double C876 = C872 / 2.;
    const double C877 = 2. * C874;
    const double C878 = 6 * C105 * C373 * C305 + 3 * std::pow(C105, 2) * C876;
    const double C879 = 0.3109069086965489e-1 * C878 + 0.;
    const double C880 = C374 * C260 + C47 * C877;
    const double C881 = C880 / 2.;
    const double C882 = C881;
    const double C883 = 2. * C882;
    const double C884 = ((-C360 * 2 * std::sqrt(C61)) / std::pow(2 * std::sqrt(C61) * C124, 2) -
                         (-C380 * std::sqrt(C61) * 2 * C124) / std::pow(C124, 4)) *
                            C324 -
                        C883 * std::sqrt(C61) / std::pow(C124, 2);
    const double C885 = 2 * C381 * C331 + 2 * C131 * C884;
    const double C886 = 0.0667245506031492 * C885;
    const double C887 = C372 * C337 + C137 * C875;
    const double C888 = C384 * C273 + C69 * C887;
    const double C889 = 0.0337738 * C888;
    const double C890 = C372 * C344 + C144 * C875;
    const double C891 = C387 * C269 + C64 * C890;
    const double C892 =
        (C889 / C66 +
         ((-C386) / std::pow(C66, 2) - (-C362 * C145 * 2 * C66) / std::pow(C66, 4)) * C270) -
        C345 * C362 / std::pow(C66, 2);
    const double C893 = C892;
    const double C894 = C891 / C65;
    const double C895 = C893 + C894;
    const double C896 =
        ((C895 / C111 +
          ((-C392) / std::pow(C111, 2) - (-C376 * C151 * 2 * C111) / std::pow(C111, 4)) * C311) -
         C879 * C151 / std::pow(C111, 2)) -
        C351 * C376 / std::pow(C111, 2);
    const double C897 = -C896;
    const double C898 = std::exp(C153) * C394 * C353 + std::exp(C153) * C897;
    const double C899 = C898;
    const double C900 = 0.3109069086965489e-1 * C899 + 0.;
    const double C901 =
        ((C886 / C156 +
          ((-C383) / std::pow(C156, 2) - (-C397 * C136 * 2 * C156) / std::pow(C156, 4)) * C356) -
         C900 * C136 / std::pow(C156, 2)) -
        C336 * C397 / std::pow(C156, 2);
    matrix.upper[6] =
        (((((((((((0. * C47 +
                   0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                 1.)) *
                      C375 +
                  C382 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.))) +
                 C398 *
                     (C47 * 0.3109069086965489e-1 * C110 *
                          (0. -
                           0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                      0. * C110 *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1))) /
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.))) +
                ((((1. * C392 + (0. * C47 + 0.3109069086965489e-1 *
                                                std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                                             (0.3109069086965489e-1 *
                                                              ((C157 + std::pow(C157, 2)) + 1.)) +
                                                         1.)) *
                                    C375) +
                   C382 *
                       (0. * C47 +
                        0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.)) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                        (0.06672455060314922 * C135 * (C157 + 1.) /
                             (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                         1.))) +
                  C398 *
                      (C47 * 0.3109069086965489e-1 * C110 *
                           (0. -
                            0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                       0.3109069086965489e-1 * C110 *
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                0.06672455060314922 * C135 -
                            0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                                (2 * C157 + 1))) /
                      (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.))) +
                 0.) *
                    C260) +
               ((((1. * C47 + (0. * C47 + 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                                            (0.3109069086965489e-1 *
                                                             ((C157 + std::pow(C157, 2)) + 1.)) +
                                                        1.)) *
                                  C375) +
                  C382 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.))) +
                 C398 *
                     (C47 * 0.3109069086965489e-1 * C110 *
                          (0. -
                           0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                      0. * C110 *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1))) /
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.))) +
                0.) *
                   C895) +
              ((((C47 * 0.2074512376219157e-2 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.)) +
                  0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                    (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                1.)) *
                     C375 +
                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.) *
                      C382 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) -
                  C382 * C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.) *
                      0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                      0.06672455060314922 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.))) /
                     std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                  (0.06672455060314922 * C135 * (C157 + 1.) /
                                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                   1.),
                              2)) +
                (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                     (0.06672455060314922 * C135 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      1.) *
                     C398 *
                     (C47 * 0.3109069086965489e-1 * C110 *
                          (0.2074512376219157e-2 * ((C157 + std::pow(C157, 2)) + 1.) -
                           0.06672455060314922 *
                               (0. * C135 * (C157 + 1.) +
                                (C157 + 1.) * 0.3109069086965489e-1 * (2 * C157 + 1))) +
                      0. * C110 *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1))) -
                 C398 * C47 * 0.3109069086965489e-1 * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1)) *
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                          0.06672455060314922 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      0. * ((C157 + std::pow(C157, 2)) + 1.) *
                          (0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.))) /
                    std::pow(
                        std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                            (0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.),
                        2)) +
               0.) *
                  C335) +
             ((((C47 * 0.3109069086965489e-1 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1)) /
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.)) +
                 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                               1.)) *
                    C375 +
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                     (0.06672455060314922 * C135 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      1.) *
                     C382 *
                     (C47 * 0.2074512376219157e-2 * C110 +
                      0. * C110 * 0.06672455060314922 * (C157 + 1.)) -
                 C382 * C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.) *
                     0.3109069086965489e-1 *
                     (((C157 + std::pow(C157, 2)) + 1.) *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1)) /
                          std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) +
                      (2 * C157 + 1) *
                          (0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.))) /
                    std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                 (0.06672455060314922 * C135 * (C157 + 1.) /
                                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                  1.),
                             2)) +
               (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                    (0.06672455060314922 * C135 * (C157 + 1.) /
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                     1.) *
                    C398 *
                    (C47 * 0.3109069086965489e-1 * C110 *
                         (0.3109069086965489e-1 * (2 * C157 + 1) * 0.06672455060314922 * C135 -
                          0.06672455060314922 * (0.6218138173930979e-1 * C135 * (C157 + 1.) +
                                                 C135 * 0.3109069086965489e-1 * (2 * C157 + 1))) +
                     0. * C110 *
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                              0.06672455060314922 * C135 -
                          0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                              (2 * C157 + 1))) -
                C398 * C47 * 0.3109069086965489e-1 * C110 *
                    (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                         0.06672455060314922 * C135 -
                     0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                         (2 * C157 + 1)) *
                    (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                              0.06672455060314922 * C135 -
                          0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                              (2 * C157 + 1)) /
                         std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) +
                     0.6218138173930979e-1 * (2 * C157 + 1) * 0.3109069086965489e-1 *
                         ((C157 + std::pow(C157, 2)) + 1.) *
                         (0.06672455060314922 * C135 * (C157 + 1.) /
                              (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                          1.))) /
                   std::pow(std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                                (0.06672455060314922 * C135 * (C157 + 1.) /
                                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                 1.),
                            2)) +
              0.) *
                 C357) +
            ((((C47 * 0.3109069086965489e-1 *
                    std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.) +
                (0. * C47 +
                 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                               1.)) *
                    C375) +
               C382 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                    (0.06672455060314922 * C135 * (C157 + 1.) /
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                     1.))) +
              C398 *
                  (C47 * 0.3109069086965489e-1 * C110 *
                       (0. -
                        0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                   0. * C110 *
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                            0.06672455060314922 * C135 -
                        0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                            (2 * C157 + 1))) /
                  (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                   (0.06672455060314922 * C135 * (C157 + 1.) /
                        (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                    1.))) +
             0.) *
                C878) +
           ((((0. * C47 +
               0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.)) *
                  C375 +
              (C382 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) +
               C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.)) /
                  (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                   (0.06672455060314922 * C135 * (C157 + 1.) /
                        (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                    1.))) +
             C398 *
                 (C47 * 0.3109069086965489e-1 * C110 *
                      (0. - 0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                  0. * C110 *
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                           0.06672455060314922 * C135 -
                       0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                           (2 * C157 + 1))) /
                 (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                  (0.06672455060314922 * C135 * (C157 + 1.) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                   1.))) +
            0.) *
               C885) +
          ((((0. * C47 +
              0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                            1.)) *
                 C375 +
             C382 *
                 (C47 * 0.2074512376219157e-2 * (C157 + 1.) +
                  0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                  (0.06672455060314922 * C135 * (C157 + 1.) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                   1.))) +
            C398 *
                (C47 * 0.3109069086965489e-1 *
                     (C110 * (0. - 0.06672455060314922 *
                                       (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                           0.06672455060314922 * C135 -
                       0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                           (2 * C157 + 1))) +
                 0. * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1))) /
                (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                 (0.06672455060314922 * C135 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                  1.))) +
           0.) *
              C310) +
         ((((0. * C47 +
             0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.)) *
                C375 +
            C382 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                 (0.06672455060314922 * C135 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                  1.))) +
           (C398 *
                (C47 * 0.3109069086965489e-1 * C110 *
                     (0. - 0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                 0. * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1))) +
            C47 * 0.3109069086965489e-1 * C110 *
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) * 0.06672455060314922 *
                     C135 -
                 0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                     (2 * C157 + 1))) /
               (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                (0.06672455060314922 * C135 * (C157 + 1.) /
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                 1.))) +
          0.) *
             C901) +
        0.;

    const double C902 = (-C260 * C400) / std::pow(C47, 2);
    const double C903 = C902;
    const double C904 = -C902;
    const double C905 = C50 * (C50 - 1) * std::pow(C67, C50 - 2) * C405 * C271 +
                        C50 * std::pow(C67, C50 - 1) * C903;
    const double C906 = C51 * (C51 - 1) * std::pow(C67, C51 - 2) * C405 * C271 +
                        C51 * std::pow(C67, C51 - 1) * C903;
    const double C907 = C50 * (C50 - 1) * std::pow(C68, C50 - 2) * C406 * C272 +
                        C50 * std::pow(C68, C50 - 1) * C904;
    const double C908 = C51 * (C51 - 1) * std::pow(C68, C51 - 2) * C406 * C272 +
                        C51 * std::pow(C68, C51 - 1) * C904;
    const double C909 = C905 + C907;
    const double C910 = C906 + C908;
    const double C911 = C276 * C411 / (2 * std::sqrt(C73)) + std::sqrt(C73) * C909;
    const double C912 = C910;
    const double C913 = C909 / 2.;
    const double C914 = 2. * C911;
    const double C915 = 6 * C105 * C415 * C305 + 3 * std::pow(C105, 2) * C913;
    const double C916 = 0.3109069086965489e-1 * C915 + 0.;
    const double C917 = C416 * C260 + C47 * C914;
    const double C918 = C917 / 2.;
    const double C919 = C918;
    const double C920 = 2. * C919;
    const double C921 = ((-C402 * 2 * std::sqrt(C61)) / std::pow(2 * std::sqrt(C61) * C124, 2) -
                         (-C422 * std::sqrt(C61) * 2 * C124) / std::pow(C124, 4)) *
                            C324 -
                        C920 * std::sqrt(C61) / std::pow(C124, 2);
    const double C922 = 2 * C423 * C331 + 2 * C131 * C921;
    const double C923 = 0.0667245506031492 * C922;
    const double C924 = C414 * C337 + C137 * C912;
    const double C925 = C426 * C273 + C69 * C924;
    const double C926 = 0.0337738 * C925;
    const double C927 = C414 * C344 + C144 * C912;
    const double C928 = C429 * C269 + C64 * C927;
    const double C929 =
        (C926 / C66 +
         ((-C428) / std::pow(C66, 2) - (-C404 * C145 * 2 * C66) / std::pow(C66, 4)) * C270) -
        C345 * C404 / std::pow(C66, 2);
    const double C930 = C929;
    const double C931 = C928 / C65;
    const double C932 = C930 + C931;
    const double C933 =
        ((C932 / C111 +
          ((-C434) / std::pow(C111, 2) - (-C418 * C151 * 2 * C111) / std::pow(C111, 4)) * C311) -
         C916 * C151 / std::pow(C111, 2)) -
        C351 * C418 / std::pow(C111, 2);
    const double C934 = -C933;
    const double C935 = std::exp(C153) * C436 * C353 + std::exp(C153) * C934;
    const double C936 = C935;
    const double C937 = 0.3109069086965489e-1 * C936 + 0.;
    const double C938 =
        ((C923 / C156 +
          ((-C425) / std::pow(C156, 2) - (-C439 * C136 * 2 * C156) / std::pow(C156, 4)) * C356) -
         C937 * C136 / std::pow(C156, 2)) -
        C336 * C439 / std::pow(C156, 2);
    matrix.upper[7] =
        (((((((((((0. * C47 +
                   0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                 1.)) *
                      C417 +
                  C424 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.))) +
                 C440 *
                     (C47 * 0.3109069086965489e-1 * C110 *
                          (0. -
                           0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                      0. * C110 *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1))) /
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.))) +
                ((((1. * C434 + (0. * C47 + 0.3109069086965489e-1 *
                                                std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                                             (0.3109069086965489e-1 *
                                                              ((C157 + std::pow(C157, 2)) + 1.)) +
                                                         1.)) *
                                    C417) +
                   C424 *
                       (0. * C47 +
                        0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.)) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                        (0.06672455060314922 * C135 * (C157 + 1.) /
                             (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                         1.))) +
                  C440 *
                      (C47 * 0.3109069086965489e-1 * C110 *
                           (0. -
                            0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                       0.3109069086965489e-1 * C110 *
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                0.06672455060314922 * C135 -
                            0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                                (2 * C157 + 1))) /
                      (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.))) +
                 0.) *
                    C260) +
               ((((1. * C47 + (0. * C47 + 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                                            (0.3109069086965489e-1 *
                                                             ((C157 + std::pow(C157, 2)) + 1.)) +
                                                        1.)) *
                                  C417) +
                  C424 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.))) +
                 C440 *
                     (C47 * 0.3109069086965489e-1 * C110 *
                          (0. -
                           0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                      0. * C110 *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1))) /
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.))) +
                0.) *
                   C932) +
              ((((C47 * 0.2074512376219157e-2 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.)) +
                  0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                    (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                1.)) *
                     C417 +
                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.) *
                      C424 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) -
                  C424 * C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.) *
                      0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                      0.06672455060314922 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.))) /
                     std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                  (0.06672455060314922 * C135 * (C157 + 1.) /
                                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                   1.),
                              2)) +
                (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                     (0.06672455060314922 * C135 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      1.) *
                     C440 *
                     (C47 * 0.3109069086965489e-1 * C110 *
                          (0.2074512376219157e-2 * ((C157 + std::pow(C157, 2)) + 1.) -
                           0.06672455060314922 *
                               (0. * C135 * (C157 + 1.) +
                                (C157 + 1.) * 0.3109069086965489e-1 * (2 * C157 + 1))) +
                      0. * C110 *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1))) -
                 C440 * C47 * 0.3109069086965489e-1 * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1)) *
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                          0.06672455060314922 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      0. * ((C157 + std::pow(C157, 2)) + 1.) *
                          (0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.))) /
                    std::pow(
                        std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                            (0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.),
                        2)) +
               0.) *
                  C335) +
             ((((C47 * 0.3109069086965489e-1 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1)) /
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.)) +
                 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                               1.)) *
                    C417 +
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                     (0.06672455060314922 * C135 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      1.) *
                     C424 *
                     (C47 * 0.2074512376219157e-2 * C110 +
                      0. * C110 * 0.06672455060314922 * (C157 + 1.)) -
                 C424 * C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.) *
                     0.3109069086965489e-1 *
                     (((C157 + std::pow(C157, 2)) + 1.) *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1)) /
                          std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) +
                      (2 * C157 + 1) *
                          (0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.))) /
                    std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                 (0.06672455060314922 * C135 * (C157 + 1.) /
                                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                  1.),
                             2)) +
               (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                    (0.06672455060314922 * C135 * (C157 + 1.) /
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                     1.) *
                    C440 *
                    (C47 * 0.3109069086965489e-1 * C110 *
                         (0.3109069086965489e-1 * (2 * C157 + 1) * 0.06672455060314922 * C135 -
                          0.06672455060314922 * (0.6218138173930979e-1 * C135 * (C157 + 1.) +
                                                 C135 * 0.3109069086965489e-1 * (2 * C157 + 1))) +
                     0. * C110 *
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                              0.06672455060314922 * C135 -
                          0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                              (2 * C157 + 1))) -
                C440 * C47 * 0.3109069086965489e-1 * C110 *
                    (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                         0.06672455060314922 * C135 -
                     0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                         (2 * C157 + 1)) *
                    (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                              0.06672455060314922 * C135 -
                          0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                              (2 * C157 + 1)) /
                         std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) +
                     0.6218138173930979e-1 * (2 * C157 + 1) * 0.3109069086965489e-1 *
                         ((C157 + std::pow(C157, 2)) + 1.) *
                         (0.06672455060314922 * C135 * (C157 + 1.) /
                              (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                          1.))) /
                   std::pow(std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                                (0.06672455060314922 * C135 * (C157 + 1.) /
                                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                 1.),
                            2)) +
              0.) *
                 C357) +
            ((((C47 * 0.3109069086965489e-1 *
                    std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.) +
                (0. * C47 +
                 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                               1.)) *
                    C417) +
               C424 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                    (0.06672455060314922 * C135 * (C157 + 1.) /
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                     1.))) +
              C440 *
                  (C47 * 0.3109069086965489e-1 * C110 *
                       (0. -
                        0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                   0. * C110 *
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                            0.06672455060314922 * C135 -
                        0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                            (2 * C157 + 1))) /
                  (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                   (0.06672455060314922 * C135 * (C157 + 1.) /
                        (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                    1.))) +
             0.) *
                C915) +
           ((((0. * C47 +
               0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.)) *
                  C417 +
              (C424 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) +
               C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.)) /
                  (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                   (0.06672455060314922 * C135 * (C157 + 1.) /
                        (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                    1.))) +
             C440 *
                 (C47 * 0.3109069086965489e-1 * C110 *
                      (0. - 0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                  0. * C110 *
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                           0.06672455060314922 * C135 -
                       0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                           (2 * C157 + 1))) /
                 (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                  (0.06672455060314922 * C135 * (C157 + 1.) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                   1.))) +
            0.) *
               C922) +
          ((((0. * C47 +
              0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                            1.)) *
                 C417 +
             C424 *
                 (C47 * 0.2074512376219157e-2 * (C157 + 1.) +
                  0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                  (0.06672455060314922 * C135 * (C157 + 1.) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                   1.))) +
            C440 *
                (C47 * 0.3109069086965489e-1 *
                     (C110 * (0. - 0.06672455060314922 *
                                       (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                           0.06672455060314922 * C135 -
                       0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                           (2 * C157 + 1))) +
                 0. * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1))) /
                (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                 (0.06672455060314922 * C135 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                  1.))) +
           0.) *
              C310) +
         ((((0. * C47 +
             0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.)) *
                C417 +
            C424 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                 (0.06672455060314922 * C135 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                  1.))) +
           (C440 *
                (C47 * 0.3109069086965489e-1 * C110 *
                     (0. - 0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                 0. * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1))) +
            C47 * 0.3109069086965489e-1 * C110 *
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) * 0.06672455060314922 *
                     C135 -
                 0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                     (2 * C157 + 1))) /
               (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                (0.06672455060314922 * C135 * (C157 + 1.) /
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                 1.))) +
          0.) *
             C938) +
        0.;

    const double C939 = (-C260 * C441) / std::pow(C47, 2);
    const double C940 = C939;
    const double C941 = -C939;
    const double C942 = C50 * (C50 - 1) * std::pow(C67, C50 - 2) * C445 * C271 +
                        C50 * std::pow(C67, C50 - 1) * C940;
    const double C943 = C51 * (C51 - 1) * std::pow(C67, C51 - 2) * C445 * C271 +
                        C51 * std::pow(C67, C51 - 1) * C940;
    const double C944 = C50 * (C50 - 1) * std::pow(C68, C50 - 2) * C446 * C272 +
                        C50 * std::pow(C68, C50 - 1) * C941;
    const double C945 = C51 * (C51 - 1) * std::pow(C68, C51 - 2) * C446 * C272 +
                        C51 * std::pow(C68, C51 - 1) * C941;
    const double C946 = C942 + C944;
    const double C947 = C943 + C945;
    const double C948 = C276 * C451 / (2 * std::sqrt(C73)) + std::sqrt(C73) * C946;
    const double C949 = C947;
    const double C950 = C946 / 2.;
    const double C951 = 2. * C948;
    const double C952 = 6 * C105 * C455 * C305 + 3 * std::pow(C105, 2) * C950;
    const double C953 = 0.3109069086965489e-1 * C952 + 0.;
    const double C954 = C456 * C260 + C47 * C951;
    const double C955 = C954 / 2.;
    const double C956 = C955;
    const double C957 = 2. * C956;
    const double C958 = ((-C442 * 2 * std::sqrt(C61)) / std::pow(2 * std::sqrt(C61) * C124, 2) -
                         (-C462 * std::sqrt(C61) * 2 * C124) / std::pow(C124, 4)) *
                            C324 -
                        C957 * std::sqrt(C61) / std::pow(C124, 2);
    const double C959 = 2 * C463 * C331 + 2 * C131 * C958;
    const double C960 = 0.0667245506031492 * C959;
    const double C961 = C454 * C337 + C137 * C949;
    const double C962 = C466 * C273 + C69 * C961;
    const double C963 = 0.0337738 * C962;
    const double C964 = C454 * C344 + C144 * C949;
    const double C965 = C469 * C269 + C64 * C964;
    const double C966 =
        (C963 / C66 +
         ((-C468) / std::pow(C66, 2) - (-C444 * C145 * 2 * C66) / std::pow(C66, 4)) * C270) -
        C345 * C444 / std::pow(C66, 2);
    const double C967 = C966;
    const double C968 = C965 / C65;
    const double C969 = C967 + C968;
    const double C970 =
        ((C969 / C111 +
          ((-C474) / std::pow(C111, 2) - (-C458 * C151 * 2 * C111) / std::pow(C111, 4)) * C311) -
         C953 * C151 / std::pow(C111, 2)) -
        C351 * C458 / std::pow(C111, 2);
    const double C971 = -C970;
    const double C972 = std::exp(C153) * C476 * C353 + std::exp(C153) * C971;
    const double C973 = C972;
    const double C974 = 0.3109069086965489e-1 * C973 + 0.;
    const double C975 =
        ((C960 / C156 +
          ((-C465) / std::pow(C156, 2) - (-C479 * C136 * 2 * C156) / std::pow(C156, 4)) * C356) -
         C974 * C136 / std::pow(C156, 2)) -
        C336 * C479 / std::pow(C156, 2);
    matrix.upper[8] =
        (((((((((((0. * C47 +
                   0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                 1.)) *
                      C457 +
                  C464 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.))) +
                 C480 *
                     (C47 * 0.3109069086965489e-1 * C110 *
                          (0. -
                           0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                      0. * C110 *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1))) /
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.))) +
                ((((1. * C474 + (0. * C47 + 0.3109069086965489e-1 *
                                                std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                                             (0.3109069086965489e-1 *
                                                              ((C157 + std::pow(C157, 2)) + 1.)) +
                                                         1.)) *
                                    C457) +
                   C464 *
                       (0. * C47 +
                        0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.)) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                        (0.06672455060314922 * C135 * (C157 + 1.) /
                             (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                         1.))) +
                  C480 *
                      (C47 * 0.3109069086965489e-1 * C110 *
                           (0. -
                            0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                       0.3109069086965489e-1 * C110 *
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                0.06672455060314922 * C135 -
                            0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                                (2 * C157 + 1))) /
                      (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.))) +
                 0.) *
                    C260) +
               ((((1. * C47 + (0. * C47 + 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                                            (0.3109069086965489e-1 *
                                                             ((C157 + std::pow(C157, 2)) + 1.)) +
                                                        1.)) *
                                  C457) +
                  C464 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.))) +
                 C480 *
                     (C47 * 0.3109069086965489e-1 * C110 *
                          (0. -
                           0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                      0. * C110 *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1))) /
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.))) +
                0.) *
                   C969) +
              ((((C47 * 0.2074512376219157e-2 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.)) +
                  0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                    (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                1.)) *
                     C457 +
                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.) *
                      C464 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) -
                  C464 * C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.) *
                      0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                      0.06672455060314922 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.))) /
                     std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                  (0.06672455060314922 * C135 * (C157 + 1.) /
                                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                   1.),
                              2)) +
                (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                     (0.06672455060314922 * C135 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      1.) *
                     C480 *
                     (C47 * 0.3109069086965489e-1 * C110 *
                          (0.2074512376219157e-2 * ((C157 + std::pow(C157, 2)) + 1.) -
                           0.06672455060314922 *
                               (0. * C135 * (C157 + 1.) +
                                (C157 + 1.) * 0.3109069086965489e-1 * (2 * C157 + 1))) +
                      0. * C110 *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1))) -
                 C480 * C47 * 0.3109069086965489e-1 * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1)) *
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                          0.06672455060314922 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      0. * ((C157 + std::pow(C157, 2)) + 1.) *
                          (0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.))) /
                    std::pow(
                        std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                            (0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.),
                        2)) +
               0.) *
                  C335) +
             ((((C47 * 0.3109069086965489e-1 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1)) /
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.)) +
                 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                               1.)) *
                    C457 +
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                     (0.06672455060314922 * C135 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      1.) *
                     C464 *
                     (C47 * 0.2074512376219157e-2 * C110 +
                      0. * C110 * 0.06672455060314922 * (C157 + 1.)) -
                 C464 * C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.) *
                     0.3109069086965489e-1 *
                     (((C157 + std::pow(C157, 2)) + 1.) *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1)) /
                          std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) +
                      (2 * C157 + 1) *
                          (0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.))) /
                    std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                 (0.06672455060314922 * C135 * (C157 + 1.) /
                                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                  1.),
                             2)) +
               (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                    (0.06672455060314922 * C135 * (C157 + 1.) /
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                     1.) *
                    C480 *
                    (C47 * 0.3109069086965489e-1 * C110 *
                         (0.3109069086965489e-1 * (2 * C157 + 1) * 0.06672455060314922 * C135 -
                          0.06672455060314922 * (0.6218138173930979e-1 * C135 * (C157 + 1.) +
                                                 C135 * 0.3109069086965489e-1 * (2 * C157 + 1))) +
                     0. * C110 *
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                              0.06672455060314922 * C135 -
                          0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                              (2 * C157 + 1))) -
                C480 * C47 * 0.3109069086965489e-1 * C110 *
                    (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                         0.06672455060314922 * C135 -
                     0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                         (2 * C157 + 1)) *
                    (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                              0.06672455060314922 * C135 -
                          0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                              (2 * C157 + 1)) /
                         std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) +
                     0.6218138173930979e-1 * (2 * C157 + 1) * 0.3109069086965489e-1 *
                         ((C157 + std::pow(C157, 2)) + 1.) *
                         (0.06672455060314922 * C135 * (C157 + 1.) /
                              (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                          1.))) /
                   std::pow(std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                                (0.06672455060314922 * C135 * (C157 + 1.) /
                                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                 1.),
                            2)) +
              0.) *
                 C357) +
            ((((C47 * 0.3109069086965489e-1 *
                    std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.) +
                (0. * C47 +
                 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                               1.)) *
                    C457) +
               C464 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                    (0.06672455060314922 * C135 * (C157 + 1.) /
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                     1.))) +
              C480 *
                  (C47 * 0.3109069086965489e-1 * C110 *
                       (0. -
                        0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                   0. * C110 *
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                            0.06672455060314922 * C135 -
                        0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                            (2 * C157 + 1))) /
                  (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                   (0.06672455060314922 * C135 * (C157 + 1.) /
                        (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                    1.))) +
             0.) *
                C952) +
           ((((0. * C47 +
               0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.)) *
                  C457 +
              (C464 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) +
               C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.)) /
                  (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                   (0.06672455060314922 * C135 * (C157 + 1.) /
                        (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                    1.))) +
             C480 *
                 (C47 * 0.3109069086965489e-1 * C110 *
                      (0. - 0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                  0. * C110 *
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                           0.06672455060314922 * C135 -
                       0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                           (2 * C157 + 1))) /
                 (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                  (0.06672455060314922 * C135 * (C157 + 1.) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                   1.))) +
            0.) *
               C959) +
          ((((0. * C47 +
              0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                            1.)) *
                 C457 +
             C464 *
                 (C47 * 0.2074512376219157e-2 * (C157 + 1.) +
                  0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                  (0.06672455060314922 * C135 * (C157 + 1.) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                   1.))) +
            C480 *
                (C47 * 0.3109069086965489e-1 *
                     (C110 * (0. - 0.06672455060314922 *
                                       (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                           0.06672455060314922 * C135 -
                       0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                           (2 * C157 + 1))) +
                 0. * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1))) /
                (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                 (0.06672455060314922 * C135 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                  1.))) +
           0.) *
              C310) +
         ((((0. * C47 +
             0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.)) *
                C457 +
            C464 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                 (0.06672455060314922 * C135 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                  1.))) +
           (C480 *
                (C47 * 0.3109069086965489e-1 * C110 *
                     (0. - 0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                 0. * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1))) +
            C47 * 0.3109069086965489e-1 * C110 *
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) * 0.06672455060314922 *
                     C135 -
                 0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                     (2 * C157 + 1))) /
               (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                (0.06672455060314922 * C135 * (C157 + 1.) /
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                 1.))) +
          0.) *
             C975) +
        0.;

    const double C976 = C50 * (C50 - 1) * std::pow(C67, C50 - 2) * std::pow(C363, 2);
    const double C977 = C51 * (C51 - 1) * std::pow(C67, C51 - 2) * std::pow(C363, 2);
    const double C978 = C50 * (C50 - 1) * std::pow(C68, C50 - 2) * std::pow(C364, 2);
    const double C979 = C51 * (C51 - 1) * std::pow(C68, C51 - 2) * std::pow(C364, 2);
    const double C980 = C976 + C978;
    const double C981 = C977 + C979;
    const double C982 = std::sqrt(C73) * C980;
    const double C983 = C981;
    const double C984 = C980 / 2.;
    const double C985 = 2. * C982;
    const double C986 = 6 * C105 * std::pow(C373, 2) + 3 * std::pow(C105, 2) * C984;
    const double C987 = 0.3109069086965489e-1 * C986 + 0.;
    const double C988 = C47 * C985;
    const double C989 = C988 / 2.;
    const double C990 = C989;
    const double C991 = 2. * C990;
    const double C992 = (((-C360 * C124 / std::sqrt(C61)) / std::pow(2 * std::sqrt(C61) * C124, 2) -
                          C380 / (2 * std::sqrt(C61) * std::pow(C124, 2))) *
                             C360 +
                         ((-C360 * 2 * std::sqrt(C61)) / std::pow(2 * std::sqrt(C61) * C124, 2) -
                          (-C380 * std::sqrt(C61) * 2 * C124) / std::pow(C124, 4)) *
                             C380) -
                        C991 * std::sqrt(C61) / std::pow(C124, 2);
    const double C993 = 2 * std::pow(C381, 2) + 2 * C131 * C992;
    const double C994 = 0.0667245506031492 * C993;
    const double C995 = C137 * C983;
    const double C996 = C69 * C995;
    const double C997 = 0.0337738 * C996;
    const double C998 = C144 * C983;
    const double C999 = C64 * C998;
    const double C1000 =
        (C997 / C66 +
         ((-C386) / std::pow(C66, 2) - (-C362 * C145 * 2 * C66) / std::pow(C66, 4)) * C362) -
        C386 * C362 / std::pow(C66, 2);
    const double C1001 = C1000;
    const double C1002 = C999 / C65;
    const double C1003 = C1001 + C1002;
    const double C1004 =
        ((C1003 / C111 +
          ((-C392) / std::pow(C111, 2) - (-C376 * C151 * 2 * C111) / std::pow(C111, 4)) * C376) -
         C987 * C151 / std::pow(C111, 2)) -
        C392 * C376 / std::pow(C111, 2);
    const double C1005 = -C1004;
    const double C1006 = std::exp(C153) * std::pow(C394, 2) + std::exp(C153) * C1005;
    const double C1007 = C1006;
    const double C1008 = 0.3109069086965489e-1 * C1007 + 0.;
    const double C1009 =
        ((C994 / C156 +
          ((-C383) / std::pow(C156, 2) - (-C397 * C136 * 2 * C156) / std::pow(C156, 4)) * C397) -
         C1008 * C136 / std::pow(C156, 2)) -
        C383 * C397 / std::pow(C156, 2);
    matrix.upper[9] =
        ((((((((((0. * C47 +
                  0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                    (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                1.)) *
                     C375 +
                 C382 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.))) +
                C398 *
                    (C47 * 0.3109069086965489e-1 * C110 *
                         (0. -
                          0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                     0. * C110 *
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                              0.06672455060314922 * C135 -
                          0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                              (2 * C157 + 1))) /
                    (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                     (0.06672455060314922 * C135 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      1.))) +
               ((((1. * C47 + (0. * C47 + 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                                            (0.3109069086965489e-1 *
                                                             ((C157 + std::pow(C157, 2)) + 1.)) +
                                                        1.)) *
                                  C375) +
                  C382 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.))) +
                 C398 *
                     (C47 * 0.3109069086965489e-1 * C110 *
                          (0. -
                           0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                      0. * C110 *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1))) /
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.))) +
                0.) *
                   C1003) +
              ((((C47 * 0.2074512376219157e-2 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.)) +
                  0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                    (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                1.)) *
                     C375 +
                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.) *
                      C382 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) -
                  C382 * C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.) *
                      0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                      0.06672455060314922 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.))) /
                     std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                  (0.06672455060314922 * C135 * (C157 + 1.) /
                                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                   1.),
                              2)) +
                (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                     (0.06672455060314922 * C135 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      1.) *
                     C398 *
                     (C47 * 0.3109069086965489e-1 * C110 *
                          (0.2074512376219157e-2 * ((C157 + std::pow(C157, 2)) + 1.) -
                           0.06672455060314922 *
                               (0. * C135 * (C157 + 1.) +
                                (C157 + 1.) * 0.3109069086965489e-1 * (2 * C157 + 1))) +
                      0. * C110 *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1))) -
                 C398 * C47 * 0.3109069086965489e-1 * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1)) *
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                          0.06672455060314922 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      0. * ((C157 + std::pow(C157, 2)) + 1.) *
                          (0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.))) /
                    std::pow(
                        std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                            (0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.),
                        2)) +
               0.) *
                  C382) +
             ((((C47 * 0.3109069086965489e-1 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1)) /
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.)) +
                 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                               1.)) *
                    C375 +
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                     (0.06672455060314922 * C135 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      1.) *
                     C382 *
                     (C47 * 0.2074512376219157e-2 * C110 +
                      0. * C110 * 0.06672455060314922 * (C157 + 1.)) -
                 C382 * C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.) *
                     0.3109069086965489e-1 *
                     (((C157 + std::pow(C157, 2)) + 1.) *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1)) /
                          std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) +
                      (2 * C157 + 1) *
                          (0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.))) /
                    std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                 (0.06672455060314922 * C135 * (C157 + 1.) /
                                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                  1.),
                             2)) +
               (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                    (0.06672455060314922 * C135 * (C157 + 1.) /
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                     1.) *
                    C398 *
                    (C47 * 0.3109069086965489e-1 * C110 *
                         (0.3109069086965489e-1 * (2 * C157 + 1) * 0.06672455060314922 * C135 -
                          0.06672455060314922 * (0.6218138173930979e-1 * C135 * (C157 + 1.) +
                                                 C135 * 0.3109069086965489e-1 * (2 * C157 + 1))) +
                     0. * C110 *
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                              0.06672455060314922 * C135 -
                          0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                              (2 * C157 + 1))) -
                C398 * C47 * 0.3109069086965489e-1 * C110 *
                    (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                         0.06672455060314922 * C135 -
                     0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                         (2 * C157 + 1)) *
                    (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                              0.06672455060314922 * C135 -
                          0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                              (2 * C157 + 1)) /
                         std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) +
                     0.6218138173930979e-1 * (2 * C157 + 1) * 0.3109069086965489e-1 *
                         ((C157 + std::pow(C157, 2)) + 1.) *
                         (0.06672455060314922 * C135 * (C157 + 1.) /
                              (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                          1.))) /
                   std::pow(std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                                (0.06672455060314922 * C135 * (C157 + 1.) /
                                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                 1.),
                            2)) +
              0.) *
                 C398) +
            ((((C47 * 0.3109069086965489e-1 *
                    std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.) +
                (0. * C47 +
                 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                               1.)) *
                    C375) +
               C382 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                    (0.06672455060314922 * C135 * (C157 + 1.) /
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                     1.))) +
              C398 *
                  (C47 * 0.3109069086965489e-1 * C110 *
                       (0. -
                        0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                   0. * C110 *
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                            0.06672455060314922 * C135 -
                        0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                            (2 * C157 + 1))) /
                  (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                   (0.06672455060314922 * C135 * (C157 + 1.) /
                        (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                    1.))) +
             0.) *
                C986) +
           ((((0. * C47 +
               0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.)) *
                  C375 +
              (C382 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) +
               C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.)) /
                  (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                   (0.06672455060314922 * C135 * (C157 + 1.) /
                        (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                    1.))) +
             C398 *
                 (C47 * 0.3109069086965489e-1 * C110 *
                      (0. - 0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                  0. * C110 *
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                           0.06672455060314922 * C135 -
                       0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                           (2 * C157 + 1))) /
                 (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                  (0.06672455060314922 * C135 * (C157 + 1.) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                   1.))) +
            0.) *
               C993) +
          ((((0. * C47 +
              0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                            1.)) *
                 C375 +
             C382 *
                 (C47 * 0.2074512376219157e-2 * (C157 + 1.) +
                  0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                  (0.06672455060314922 * C135 * (C157 + 1.) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                   1.))) +
            C398 *
                (C47 * 0.3109069086965489e-1 *
                     (C110 * (0. - 0.06672455060314922 *
                                       (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                           0.06672455060314922 * C135 -
                       0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                           (2 * C157 + 1))) +
                 0. * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1))) /
                (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                 (0.06672455060314922 * C135 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                  1.))) +
           0.) *
              C375) +
         ((((0. * C47 +
             0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.)) *
                C375 +
            C382 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                 (0.06672455060314922 * C135 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                  1.))) +
           (C398 *
                (C47 * 0.3109069086965489e-1 * C110 *
                     (0. - 0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                 0. * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1))) +
            C47 * 0.3109069086965489e-1 * C110 *
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) * 0.06672455060314922 *
                     C135 -
                 0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                     (2 * C157 + 1))) /
               (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                (0.06672455060314922 * C135 * (C157 + 1.) /
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                 1.))) +
          0.) *
             C1009) +
        0.;

    const double C1010 = C50 * (C50 - 1) * std::pow(C67, C50 - 2) * C363 * C405;
    const double C1011 = C51 * (C51 - 1) * std::pow(C67, C51 - 2) * C363 * C405;
    const double C1012 = C50 * (C50 - 1) * std::pow(C68, C50 - 2) * C364 * C406;
    const double C1013 = C51 * (C51 - 1) * std::pow(C68, C51 - 2) * C364 * C406;
    const double C1014 = C1010 + C1012;
    const double C1015 = C1011 + C1013;
    const double C1016 = std::sqrt(C73) * C1014;
    const double C1017 = C1015;
    const double C1018 = C1014 / 2.;
    const double C1019 = 2. * C1016;
    const double C1020 = 6 * C105 * C373 * C415 + 3 * std::pow(C105, 2) * C1018;
    const double C1021 = 0.3109069086965489e-1 * C1020 + 0.;
    const double C1022 = C47 * C1019;
    const double C1023 = C1022 / 2.;
    const double C1024 = C1023;
    const double C1025 = 2. * C1024;
    const double C1026 =
        (((-C360 * C124 / std::sqrt(C61)) / std::pow(2 * std::sqrt(C61) * C124, 2) -
          C380 / (2 * std::sqrt(C61) * std::pow(C124, 2))) *
             C402 +
         ((-C360 * 2 * std::sqrt(C61)) / std::pow(2 * std::sqrt(C61) * C124, 2) -
          (-C380 * std::sqrt(C61) * 2 * C124) / std::pow(C124, 4)) *
             C422) -
        C1025 * std::sqrt(C61) / std::pow(C124, 2);
    const double C1027 = 2 * C381 * C423 + 2 * C131 * C1026;
    const double C1028 = 0.0667245506031492 * C1027;
    const double C1029 = C137 * C1017;
    const double C1030 = C69 * C1029;
    const double C1031 = 0.0337738 * C1030;
    const double C1032 = C144 * C1017;
    const double C1033 = C64 * C1032;
    const double C1034 =
        (C1031 / C66 +
         ((-C386) / std::pow(C66, 2) - (-C362 * C145 * 2 * C66) / std::pow(C66, 4)) * C404) -
        C428 * C362 / std::pow(C66, 2);
    const double C1035 = C1034;
    const double C1036 = C1033 / C65;
    const double C1037 = C1035 + C1036;
    const double C1038 =
        ((C1037 / C111 +
          ((-C392) / std::pow(C111, 2) - (-C376 * C151 * 2 * C111) / std::pow(C111, 4)) * C418) -
         C1021 * C151 / std::pow(C111, 2)) -
        C434 * C376 / std::pow(C111, 2);
    const double C1039 = -C1038;
    const double C1040 = std::exp(C153) * C394 * C436 + std::exp(C153) * C1039;
    const double C1041 = C1040;
    const double C1042 = 0.3109069086965489e-1 * C1041 + 0.;
    const double C1043 =
        ((C1028 / C156 +
          ((-C383) / std::pow(C156, 2) - (-C397 * C136 * 2 * C156) / std::pow(C156, 4)) * C439) -
         C1042 * C136 / std::pow(C156, 2)) -
        C425 * C397 / std::pow(C156, 2);
    matrix.upper[10] =
        ((((((((((0. * C47 +
                  0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                    (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                1.)) *
                     C375 +
                 C382 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.))) +
                C398 *
                    (C47 * 0.3109069086965489e-1 * C110 *
                         (0. -
                          0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                     0. * C110 *
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                              0.06672455060314922 * C135 -
                          0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                              (2 * C157 + 1))) /
                    (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                     (0.06672455060314922 * C135 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      1.))) +
               ((((1. * C47 + (0. * C47 + 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                                            (0.3109069086965489e-1 *
                                                             ((C157 + std::pow(C157, 2)) + 1.)) +
                                                        1.)) *
                                  C375) +
                  C382 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.))) +
                 C398 *
                     (C47 * 0.3109069086965489e-1 * C110 *
                          (0. -
                           0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                      0. * C110 *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1))) /
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.))) +
                0.) *
                   C1037) +
              ((((C47 * 0.2074512376219157e-2 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.)) +
                  0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                    (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                1.)) *
                     C375 +
                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.) *
                      C382 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) -
                  C382 * C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.) *
                      0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                      0.06672455060314922 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.))) /
                     std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                  (0.06672455060314922 * C135 * (C157 + 1.) /
                                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                   1.),
                              2)) +
                (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                     (0.06672455060314922 * C135 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      1.) *
                     C398 *
                     (C47 * 0.3109069086965489e-1 * C110 *
                          (0.2074512376219157e-2 * ((C157 + std::pow(C157, 2)) + 1.) -
                           0.06672455060314922 *
                               (0. * C135 * (C157 + 1.) +
                                (C157 + 1.) * 0.3109069086965489e-1 * (2 * C157 + 1))) +
                      0. * C110 *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1))) -
                 C398 * C47 * 0.3109069086965489e-1 * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1)) *
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                          0.06672455060314922 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      0. * ((C157 + std::pow(C157, 2)) + 1.) *
                          (0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.))) /
                    std::pow(
                        std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                            (0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.),
                        2)) +
               0.) *
                  C424) +
             ((((C47 * 0.3109069086965489e-1 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1)) /
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.)) +
                 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                               1.)) *
                    C375 +
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                     (0.06672455060314922 * C135 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      1.) *
                     C382 *
                     (C47 * 0.2074512376219157e-2 * C110 +
                      0. * C110 * 0.06672455060314922 * (C157 + 1.)) -
                 C382 * C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.) *
                     0.3109069086965489e-1 *
                     (((C157 + std::pow(C157, 2)) + 1.) *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1)) /
                          std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) +
                      (2 * C157 + 1) *
                          (0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.))) /
                    std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                 (0.06672455060314922 * C135 * (C157 + 1.) /
                                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                  1.),
                             2)) +
               (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                    (0.06672455060314922 * C135 * (C157 + 1.) /
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                     1.) *
                    C398 *
                    (C47 * 0.3109069086965489e-1 * C110 *
                         (0.3109069086965489e-1 * (2 * C157 + 1) * 0.06672455060314922 * C135 -
                          0.06672455060314922 * (0.6218138173930979e-1 * C135 * (C157 + 1.) +
                                                 C135 * 0.3109069086965489e-1 * (2 * C157 + 1))) +
                     0. * C110 *
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                              0.06672455060314922 * C135 -
                          0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                              (2 * C157 + 1))) -
                C398 * C47 * 0.3109069086965489e-1 * C110 *
                    (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                         0.06672455060314922 * C135 -
                     0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                         (2 * C157 + 1)) *
                    (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                              0.06672455060314922 * C135 -
                          0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                              (2 * C157 + 1)) /
                         std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) +
                     0.6218138173930979e-1 * (2 * C157 + 1) * 0.3109069086965489e-1 *
                         ((C157 + std::pow(C157, 2)) + 1.) *
                         (0.06672455060314922 * C135 * (C157 + 1.) /
                              (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                          1.))) /
                   std::pow(std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                                (0.06672455060314922 * C135 * (C157 + 1.) /
                                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                 1.),
                            2)) +
              0.) *
                 C440) +
            ((((C47 * 0.3109069086965489e-1 *
                    std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.) +
                (0. * C47 +
                 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                               1.)) *
                    C375) +
               C382 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                    (0.06672455060314922 * C135 * (C157 + 1.) /
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                     1.))) +
              C398 *
                  (C47 * 0.3109069086965489e-1 * C110 *
                       (0. -
                        0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                   0. * C110 *
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                            0.06672455060314922 * C135 -
                        0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                            (2 * C157 + 1))) /
                  (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                   (0.06672455060314922 * C135 * (C157 + 1.) /
                        (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                    1.))) +
             0.) *
                C1020) +
           ((((0. * C47 +
               0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.)) *
                  C375 +
              (C382 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) +
               C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.)) /
                  (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                   (0.06672455060314922 * C135 * (C157 + 1.) /
                        (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                    1.))) +
             C398 *
                 (C47 * 0.3109069086965489e-1 * C110 *
                      (0. - 0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                  0. * C110 *
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                           0.06672455060314922 * C135 -
                       0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                           (2 * C157 + 1))) /
                 (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                  (0.06672455060314922 * C135 * (C157 + 1.) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                   1.))) +
            0.) *
               C1027) +
          ((((0. * C47 +
              0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                            1.)) *
                 C375 +
             C382 *
                 (C47 * 0.2074512376219157e-2 * (C157 + 1.) +
                  0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                  (0.06672455060314922 * C135 * (C157 + 1.) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                   1.))) +
            C398 *
                (C47 * 0.3109069086965489e-1 *
                     (C110 * (0. - 0.06672455060314922 *
                                       (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                           0.06672455060314922 * C135 -
                       0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                           (2 * C157 + 1))) +
                 0. * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1))) /
                (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                 (0.06672455060314922 * C135 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                  1.))) +
           0.) *
              C417) +
         ((((0. * C47 +
             0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.)) *
                C375 +
            C382 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                 (0.06672455060314922 * C135 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                  1.))) +
           (C398 *
                (C47 * 0.3109069086965489e-1 * C110 *
                     (0. - 0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                 0. * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1))) +
            C47 * 0.3109069086965489e-1 * C110 *
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) * 0.06672455060314922 *
                     C135 -
                 0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                     (2 * C157 + 1))) /
               (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                (0.06672455060314922 * C135 * (C157 + 1.) /
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                 1.))) +
          0.) *
             C1043) +
        0.;

    const double C1044 = C50 * (C50 - 1) * std::pow(C67, C50 - 2) * C363 * C445;
    const double C1045 = C51 * (C51 - 1) * std::pow(C67, C51 - 2) * C363 * C445;
    const double C1046 = C50 * (C50 - 1) * std::pow(C68, C50 - 2) * C364 * C446;
    const double C1047 = C51 * (C51 - 1) * std::pow(C68, C51 - 2) * C364 * C446;
    const double C1048 = C1044 + C1046;
    const double C1049 = C1045 + C1047;
    const double C1050 = std::sqrt(C73) * C1048;
    const double C1051 = C1049;
    const double C1052 = C1048 / 2.;
    const double C1053 = 2. * C1050;
    const double C1054 = 6 * C105 * C373 * C455 + 3 * std::pow(C105, 2) * C1052;
    const double C1055 = 0.3109069086965489e-1 * C1054 + 0.;
    const double C1056 = C47 * C1053;
    const double C1057 = C1056 / 2.;
    const double C1058 = C1057;
    const double C1059 = 2. * C1058;
    const double C1060 =
        (((-C360 * C124 / std::sqrt(C61)) / std::pow(2 * std::sqrt(C61) * C124, 2) -
          C380 / (2 * std::sqrt(C61) * std::pow(C124, 2))) *
             C442 +
         ((-C360 * 2 * std::sqrt(C61)) / std::pow(2 * std::sqrt(C61) * C124, 2) -
          (-C380 * std::sqrt(C61) * 2 * C124) / std::pow(C124, 4)) *
             C462) -
        C1059 * std::sqrt(C61) / std::pow(C124, 2);
    const double C1061 = 2 * C381 * C463 + 2 * C131 * C1060;
    const double C1062 = 0.0667245506031492 * C1061;
    const double C1063 = C137 * C1051;
    const double C1064 = C69 * C1063;
    const double C1065 = 0.0337738 * C1064;
    const double C1066 = C144 * C1051;
    const double C1067 = C64 * C1066;
    const double C1068 =
        (C1065 / C66 +
         ((-C386) / std::pow(C66, 2) - (-C362 * C145 * 2 * C66) / std::pow(C66, 4)) * C444) -
        C468 * C362 / std::pow(C66, 2);
    const double C1069 = C1068;
    const double C1070 = C1067 / C65;
    const double C1071 = C1069 + C1070;
    const double C1072 =
        ((C1071 / C111 +
          ((-C392) / std::pow(C111, 2) - (-C376 * C151 * 2 * C111) / std::pow(C111, 4)) * C458) -
         C1055 * C151 / std::pow(C111, 2)) -
        C474 * C376 / std::pow(C111, 2);
    const double C1073 = -C1072;
    const double C1074 = std::exp(C153) * C394 * C476 + std::exp(C153) * C1073;
    const double C1075 = C1074;
    const double C1076 = 0.3109069086965489e-1 * C1075 + 0.;
    const double C1077 =
        ((C1062 / C156 +
          ((-C383) / std::pow(C156, 2) - (-C397 * C136 * 2 * C156) / std::pow(C156, 4)) * C479) -
         C1076 * C136 / std::pow(C156, 2)) -
        C465 * C397 / std::pow(C156, 2);
    matrix.upper[11] =
        ((((((((((0. * C47 +
                  0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                    (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                1.)) *
                     C375 +
                 C382 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.))) +
                C398 *
                    (C47 * 0.3109069086965489e-1 * C110 *
                         (0. -
                          0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                     0. * C110 *
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                              0.06672455060314922 * C135 -
                          0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                              (2 * C157 + 1))) /
                    (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                     (0.06672455060314922 * C135 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      1.))) +
               ((((1. * C47 + (0. * C47 + 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                                            (0.3109069086965489e-1 *
                                                             ((C157 + std::pow(C157, 2)) + 1.)) +
                                                        1.)) *
                                  C375) +
                  C382 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.))) +
                 C398 *
                     (C47 * 0.3109069086965489e-1 * C110 *
                          (0. -
                           0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                      0. * C110 *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1))) /
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.))) +
                0.) *
                   C1071) +
              ((((C47 * 0.2074512376219157e-2 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.)) +
                  0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                    (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                1.)) *
                     C375 +
                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.) *
                      C382 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) -
                  C382 * C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.) *
                      0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                      0.06672455060314922 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.))) /
                     std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                  (0.06672455060314922 * C135 * (C157 + 1.) /
                                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                   1.),
                              2)) +
                (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                     (0.06672455060314922 * C135 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      1.) *
                     C398 *
                     (C47 * 0.3109069086965489e-1 * C110 *
                          (0.2074512376219157e-2 * ((C157 + std::pow(C157, 2)) + 1.) -
                           0.06672455060314922 *
                               (0. * C135 * (C157 + 1.) +
                                (C157 + 1.) * 0.3109069086965489e-1 * (2 * C157 + 1))) +
                      0. * C110 *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1))) -
                 C398 * C47 * 0.3109069086965489e-1 * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1)) *
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                          0.06672455060314922 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      0. * ((C157 + std::pow(C157, 2)) + 1.) *
                          (0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.))) /
                    std::pow(
                        std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                            (0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.),
                        2)) +
               0.) *
                  C464) +
             ((((C47 * 0.3109069086965489e-1 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1)) /
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.)) +
                 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                               1.)) *
                    C375 +
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                     (0.06672455060314922 * C135 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      1.) *
                     C382 *
                     (C47 * 0.2074512376219157e-2 * C110 +
                      0. * C110 * 0.06672455060314922 * (C157 + 1.)) -
                 C382 * C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.) *
                     0.3109069086965489e-1 *
                     (((C157 + std::pow(C157, 2)) + 1.) *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1)) /
                          std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) +
                      (2 * C157 + 1) *
                          (0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.))) /
                    std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                 (0.06672455060314922 * C135 * (C157 + 1.) /
                                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                  1.),
                             2)) +
               (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                    (0.06672455060314922 * C135 * (C157 + 1.) /
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                     1.) *
                    C398 *
                    (C47 * 0.3109069086965489e-1 * C110 *
                         (0.3109069086965489e-1 * (2 * C157 + 1) * 0.06672455060314922 * C135 -
                          0.06672455060314922 * (0.6218138173930979e-1 * C135 * (C157 + 1.) +
                                                 C135 * 0.3109069086965489e-1 * (2 * C157 + 1))) +
                     0. * C110 *
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                              0.06672455060314922 * C135 -
                          0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                              (2 * C157 + 1))) -
                C398 * C47 * 0.3109069086965489e-1 * C110 *
                    (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                         0.06672455060314922 * C135 -
                     0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                         (2 * C157 + 1)) *
                    (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                              0.06672455060314922 * C135 -
                          0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                              (2 * C157 + 1)) /
                         std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) +
                     0.6218138173930979e-1 * (2 * C157 + 1) * 0.3109069086965489e-1 *
                         ((C157 + std::pow(C157, 2)) + 1.) *
                         (0.06672455060314922 * C135 * (C157 + 1.) /
                              (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                          1.))) /
                   std::pow(std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                                (0.06672455060314922 * C135 * (C157 + 1.) /
                                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                 1.),
                            2)) +
              0.) *
                 C480) +
            ((((C47 * 0.3109069086965489e-1 *
                    std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.) +
                (0. * C47 +
                 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                               1.)) *
                    C375) +
               C382 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                    (0.06672455060314922 * C135 * (C157 + 1.) /
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                     1.))) +
              C398 *
                  (C47 * 0.3109069086965489e-1 * C110 *
                       (0. -
                        0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                   0. * C110 *
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                            0.06672455060314922 * C135 -
                        0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                            (2 * C157 + 1))) /
                  (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                   (0.06672455060314922 * C135 * (C157 + 1.) /
                        (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                    1.))) +
             0.) *
                C1054) +
           ((((0. * C47 +
               0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.)) *
                  C375 +
              (C382 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) +
               C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.)) /
                  (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                   (0.06672455060314922 * C135 * (C157 + 1.) /
                        (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                    1.))) +
             C398 *
                 (C47 * 0.3109069086965489e-1 * C110 *
                      (0. - 0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                  0. * C110 *
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                           0.06672455060314922 * C135 -
                       0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                           (2 * C157 + 1))) /
                 (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                  (0.06672455060314922 * C135 * (C157 + 1.) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                   1.))) +
            0.) *
               C1061) +
          ((((0. * C47 +
              0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                            1.)) *
                 C375 +
             C382 *
                 (C47 * 0.2074512376219157e-2 * (C157 + 1.) +
                  0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                  (0.06672455060314922 * C135 * (C157 + 1.) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                   1.))) +
            C398 *
                (C47 * 0.3109069086965489e-1 *
                     (C110 * (0. - 0.06672455060314922 *
                                       (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                           0.06672455060314922 * C135 -
                       0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                           (2 * C157 + 1))) +
                 0. * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1))) /
                (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                 (0.06672455060314922 * C135 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                  1.))) +
           0.) *
              C457) +
         ((((0. * C47 +
             0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.)) *
                C375 +
            C382 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                 (0.06672455060314922 * C135 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                  1.))) +
           (C398 *
                (C47 * 0.3109069086965489e-1 * C110 *
                     (0. - 0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                 0. * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1))) +
            C47 * 0.3109069086965489e-1 * C110 *
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) * 0.06672455060314922 *
                     C135 -
                 0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                     (2 * C157 + 1))) /
               (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                (0.06672455060314922 * C135 * (C157 + 1.) /
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                 1.))) +
          0.) *
             C1077) +
        0.;

    const double C1078 = C50 * (C50 - 1) * std::pow(C67, C50 - 2) * std::pow(C405, 2);
    const double C1079 = C51 * (C51 - 1) * std::pow(C67, C51 - 2) * std::pow(C405, 2);
    const double C1080 = C50 * (C50 - 1) * std::pow(C68, C50 - 2) * std::pow(C406, 2);
    const double C1081 = C51 * (C51 - 1) * std::pow(C68, C51 - 2) * std::pow(C406, 2);
    const double C1082 = C1078 + C1080;
    const double C1083 = C1079 + C1081;
    const double C1084 = std::sqrt(C73) * C1082;
    const double C1085 = C1083;
    const double C1086 = C1082 / 2.;
    const double C1087 = 2. * C1084;
    const double C1088 = 6 * C105 * std::pow(C415, 2) + 3 * std::pow(C105, 2) * C1086;
    const double C1089 = 0.3109069086965489e-1 * C1088 + 0.;
    const double C1090 = C47 * C1087;
    const double C1091 = C1090 / 2.;
    const double C1092 = C1091;
    const double C1093 = 2. * C1092;
    const double C1094 =
        (((-C402 * C124 / std::sqrt(C61)) / std::pow(2 * std::sqrt(C61) * C124, 2) -
          C422 / (2 * std::sqrt(C61) * std::pow(C124, 2))) *
             C402 +
         ((-C402 * 2 * std::sqrt(C61)) / std::pow(2 * std::sqrt(C61) * C124, 2) -
          (-C422 * std::sqrt(C61) * 2 * C124) / std::pow(C124, 4)) *
             C422) -
        C1093 * std::sqrt(C61) / std::pow(C124, 2);
    const double C1095 = 2 * std::pow(C423, 2) + 2 * C131 * C1094;
    const double C1096 = 0.0667245506031492 * C1095;
    const double C1097 = C137 * C1085;
    const double C1098 = C69 * C1097;
    const double C1099 = 0.0337738 * C1098;
    const double C1100 = C144 * C1085;
    const double C1101 = C64 * C1100;
    const double C1102 =
        (C1099 / C66 +
         ((-C428) / std::pow(C66, 2) - (-C404 * C145 * 2 * C66) / std::pow(C66, 4)) * C404) -
        C428 * C404 / std::pow(C66, 2);
    const double C1103 = C1102;
    const double C1104 = C1101 / C65;
    const double C1105 = C1103 + C1104;
    const double C1106 =
        ((C1105 / C111 +
          ((-C434) / std::pow(C111, 2) - (-C418 * C151 * 2 * C111) / std::pow(C111, 4)) * C418) -
         C1089 * C151 / std::pow(C111, 2)) -
        C434 * C418 / std::pow(C111, 2);
    const double C1107 = -C1106;
    const double C1108 = std::exp(C153) * std::pow(C436, 2) + std::exp(C153) * C1107;
    const double C1109 = C1108;
    const double C1110 = 0.3109069086965489e-1 * C1109 + 0.;
    const double C1111 =
        ((C1096 / C156 +
          ((-C425) / std::pow(C156, 2) - (-C439 * C136 * 2 * C156) / std::pow(C156, 4)) * C439) -
         C1110 * C136 / std::pow(C156, 2)) -
        C425 * C439 / std::pow(C156, 2);
    matrix.upper[12] =
        ((((((((((0. * C47 +
                  0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                    (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                1.)) *
                     C417 +
                 C424 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.))) +
                C440 *
                    (C47 * 0.3109069086965489e-1 * C110 *
                         (0. -
                          0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                     0. * C110 *
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                              0.06672455060314922 * C135 -
                          0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                              (2 * C157 + 1))) /
                    (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                     (0.06672455060314922 * C135 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      1.))) +
               ((((1. * C47 + (0. * C47 + 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                                            (0.3109069086965489e-1 *
                                                             ((C157 + std::pow(C157, 2)) + 1.)) +
                                                        1.)) *
                                  C417) +
                  C424 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.))) +
                 C440 *
                     (C47 * 0.3109069086965489e-1 * C110 *
                          (0. -
                           0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                      0. * C110 *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1))) /
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.))) +
                0.) *
                   C1105) +
              ((((C47 * 0.2074512376219157e-2 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.)) +
                  0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                    (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                1.)) *
                     C417 +
                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.) *
                      C424 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) -
                  C424 * C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.) *
                      0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                      0.06672455060314922 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.))) /
                     std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                  (0.06672455060314922 * C135 * (C157 + 1.) /
                                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                   1.),
                              2)) +
                (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                     (0.06672455060314922 * C135 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      1.) *
                     C440 *
                     (C47 * 0.3109069086965489e-1 * C110 *
                          (0.2074512376219157e-2 * ((C157 + std::pow(C157, 2)) + 1.) -
                           0.06672455060314922 *
                               (0. * C135 * (C157 + 1.) +
                                (C157 + 1.) * 0.3109069086965489e-1 * (2 * C157 + 1))) +
                      0. * C110 *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1))) -
                 C440 * C47 * 0.3109069086965489e-1 * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1)) *
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                          0.06672455060314922 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      0. * ((C157 + std::pow(C157, 2)) + 1.) *
                          (0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.))) /
                    std::pow(
                        std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                            (0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.),
                        2)) +
               0.) *
                  C424) +
             ((((C47 * 0.3109069086965489e-1 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1)) /
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.)) +
                 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                               1.)) *
                    C417 +
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                     (0.06672455060314922 * C135 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      1.) *
                     C424 *
                     (C47 * 0.2074512376219157e-2 * C110 +
                      0. * C110 * 0.06672455060314922 * (C157 + 1.)) -
                 C424 * C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.) *
                     0.3109069086965489e-1 *
                     (((C157 + std::pow(C157, 2)) + 1.) *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1)) /
                          std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) +
                      (2 * C157 + 1) *
                          (0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.))) /
                    std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                 (0.06672455060314922 * C135 * (C157 + 1.) /
                                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                  1.),
                             2)) +
               (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                    (0.06672455060314922 * C135 * (C157 + 1.) /
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                     1.) *
                    C440 *
                    (C47 * 0.3109069086965489e-1 * C110 *
                         (0.3109069086965489e-1 * (2 * C157 + 1) * 0.06672455060314922 * C135 -
                          0.06672455060314922 * (0.6218138173930979e-1 * C135 * (C157 + 1.) +
                                                 C135 * 0.3109069086965489e-1 * (2 * C157 + 1))) +
                     0. * C110 *
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                              0.06672455060314922 * C135 -
                          0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                              (2 * C157 + 1))) -
                C440 * C47 * 0.3109069086965489e-1 * C110 *
                    (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                         0.06672455060314922 * C135 -
                     0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                         (2 * C157 + 1)) *
                    (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                              0.06672455060314922 * C135 -
                          0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                              (2 * C157 + 1)) /
                         std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) +
                     0.6218138173930979e-1 * (2 * C157 + 1) * 0.3109069086965489e-1 *
                         ((C157 + std::pow(C157, 2)) + 1.) *
                         (0.06672455060314922 * C135 * (C157 + 1.) /
                              (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                          1.))) /
                   std::pow(std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                                (0.06672455060314922 * C135 * (C157 + 1.) /
                                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                 1.),
                            2)) +
              0.) *
                 C440) +
            ((((C47 * 0.3109069086965489e-1 *
                    std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.) +
                (0. * C47 +
                 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                               1.)) *
                    C417) +
               C424 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                    (0.06672455060314922 * C135 * (C157 + 1.) /
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                     1.))) +
              C440 *
                  (C47 * 0.3109069086965489e-1 * C110 *
                       (0. -
                        0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                   0. * C110 *
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                            0.06672455060314922 * C135 -
                        0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                            (2 * C157 + 1))) /
                  (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                   (0.06672455060314922 * C135 * (C157 + 1.) /
                        (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                    1.))) +
             0.) *
                C1088) +
           ((((0. * C47 +
               0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.)) *
                  C417 +
              (C424 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) +
               C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.)) /
                  (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                   (0.06672455060314922 * C135 * (C157 + 1.) /
                        (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                    1.))) +
             C440 *
                 (C47 * 0.3109069086965489e-1 * C110 *
                      (0. - 0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                  0. * C110 *
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                           0.06672455060314922 * C135 -
                       0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                           (2 * C157 + 1))) /
                 (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                  (0.06672455060314922 * C135 * (C157 + 1.) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                   1.))) +
            0.) *
               C1095) +
          ((((0. * C47 +
              0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                            1.)) *
                 C417 +
             C424 *
                 (C47 * 0.2074512376219157e-2 * (C157 + 1.) +
                  0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                  (0.06672455060314922 * C135 * (C157 + 1.) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                   1.))) +
            C440 *
                (C47 * 0.3109069086965489e-1 *
                     (C110 * (0. - 0.06672455060314922 *
                                       (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                           0.06672455060314922 * C135 -
                       0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                           (2 * C157 + 1))) +
                 0. * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1))) /
                (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                 (0.06672455060314922 * C135 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                  1.))) +
           0.) *
              C417) +
         ((((0. * C47 +
             0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.)) *
                C417 +
            C424 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                 (0.06672455060314922 * C135 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                  1.))) +
           (C440 *
                (C47 * 0.3109069086965489e-1 * C110 *
                     (0. - 0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                 0. * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1))) +
            C47 * 0.3109069086965489e-1 * C110 *
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) * 0.06672455060314922 *
                     C135 -
                 0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                     (2 * C157 + 1))) /
               (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                (0.06672455060314922 * C135 * (C157 + 1.) /
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                 1.))) +
          0.) *
             C1111) +
        0.;

    const double C1112 = C50 * (C50 - 1) * std::pow(C67, C50 - 2) * C405 * C445;
    const double C1113 = C51 * (C51 - 1) * std::pow(C67, C51 - 2) * C405 * C445;
    const double C1114 = C50 * (C50 - 1) * std::pow(C68, C50 - 2) * C406 * C446;
    const double C1115 = C51 * (C51 - 1) * std::pow(C68, C51 - 2) * C406 * C446;
    const double C1116 = C1112 + C1114;
    const double C1117 = C1113 + C1115;
    const double C1118 = std::sqrt(C73) * C1116;
    const double C1119 = C1117;
    const double C1120 = C1116 / 2.;
    const double C1121 = 2. * C1118;
    const double C1122 = 6 * C105 * C415 * C455 + 3 * std::pow(C105, 2) * C1120;
    const double C1123 = 0.3109069086965489e-1 * C1122 + 0.;
    const double C1124 = C47 * C1121;
    const double C1125 = C1124 / 2.;
    const double C1126 = C1125;
    const double C1127 = 2. * C1126;
    const double C1128 =
        (((-C402 * C124 / std::sqrt(C61)) / std::pow(2 * std::sqrt(C61) * C124, 2) -
          C422 / (2 * std::sqrt(C61) * std::pow(C124, 2))) *
             C442 +
         ((-C402 * 2 * std::sqrt(C61)) / std::pow(2 * std::sqrt(C61) * C124, 2) -
          (-C422 * std::sqrt(C61) * 2 * C124) / std::pow(C124, 4)) *
             C462) -
        C1127 * std::sqrt(C61) / std::pow(C124, 2);
    const double C1129 = 2 * C423 * C463 + 2 * C131 * C1128;
    const double C1130 = 0.0667245506031492 * C1129;
    const double C1131 = C137 * C1119;
    const double C1132 = C69 * C1131;
    const double C1133 = 0.0337738 * C1132;
    const double C1134 = C144 * C1119;
    const double C1135 = C64 * C1134;
    const double C1136 =
        (C1133 / C66 +
         ((-C428) / std::pow(C66, 2) - (-C404 * C145 * 2 * C66) / std::pow(C66, 4)) * C444) -
        C468 * C404 / std::pow(C66, 2);
    const double C1137 = C1136;
    const double C1138 = C1135 / C65;
    const double C1139 = C1137 + C1138;
    const double C1140 =
        ((C1139 / C111 +
          ((-C434) / std::pow(C111, 2) - (-C418 * C151 * 2 * C111) / std::pow(C111, 4)) * C458) -
         C1123 * C151 / std::pow(C111, 2)) -
        C474 * C418 / std::pow(C111, 2);
    const double C1141 = -C1140;
    const double C1142 = std::exp(C153) * C436 * C476 + std::exp(C153) * C1141;
    const double C1143 = C1142;
    const double C1144 = 0.3109069086965489e-1 * C1143 + 0.;
    const double C1145 =
        ((C1130 / C156 +
          ((-C425) / std::pow(C156, 2) - (-C439 * C136 * 2 * C156) / std::pow(C156, 4)) * C479) -
         C1144 * C136 / std::pow(C156, 2)) -
        C465 * C439 / std::pow(C156, 2);
    matrix.upper[13] =
        ((((((((((0. * C47 +
                  0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                    (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                1.)) *
                     C417 +
                 C424 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.))) +
                C440 *
                    (C47 * 0.3109069086965489e-1 * C110 *
                         (0. -
                          0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                     0. * C110 *
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                              0.06672455060314922 * C135 -
                          0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                              (2 * C157 + 1))) /
                    (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                     (0.06672455060314922 * C135 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      1.))) +
               ((((1. * C47 + (0. * C47 + 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                                            (0.3109069086965489e-1 *
                                                             ((C157 + std::pow(C157, 2)) + 1.)) +
                                                        1.)) *
                                  C417) +
                  C424 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.))) +
                 C440 *
                     (C47 * 0.3109069086965489e-1 * C110 *
                          (0. -
                           0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                      0. * C110 *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1))) /
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.))) +
                0.) *
                   C1139) +
              ((((C47 * 0.2074512376219157e-2 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.)) +
                  0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                    (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                1.)) *
                     C417 +
                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.) *
                      C424 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) -
                  C424 * C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.) *
                      0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                      0.06672455060314922 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.))) /
                     std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                  (0.06672455060314922 * C135 * (C157 + 1.) /
                                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                   1.),
                              2)) +
                (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                     (0.06672455060314922 * C135 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      1.) *
                     C440 *
                     (C47 * 0.3109069086965489e-1 * C110 *
                          (0.2074512376219157e-2 * ((C157 + std::pow(C157, 2)) + 1.) -
                           0.06672455060314922 *
                               (0. * C135 * (C157 + 1.) +
                                (C157 + 1.) * 0.3109069086965489e-1 * (2 * C157 + 1))) +
                      0. * C110 *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1))) -
                 C440 * C47 * 0.3109069086965489e-1 * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1)) *
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                          0.06672455060314922 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      0. * ((C157 + std::pow(C157, 2)) + 1.) *
                          (0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.))) /
                    std::pow(
                        std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                            (0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.),
                        2)) +
               0.) *
                  C464) +
             ((((C47 * 0.3109069086965489e-1 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1)) /
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.)) +
                 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                               1.)) *
                    C417 +
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                     (0.06672455060314922 * C135 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      1.) *
                     C424 *
                     (C47 * 0.2074512376219157e-2 * C110 +
                      0. * C110 * 0.06672455060314922 * (C157 + 1.)) -
                 C424 * C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.) *
                     0.3109069086965489e-1 *
                     (((C157 + std::pow(C157, 2)) + 1.) *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1)) /
                          std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) +
                      (2 * C157 + 1) *
                          (0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.))) /
                    std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                 (0.06672455060314922 * C135 * (C157 + 1.) /
                                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                  1.),
                             2)) +
               (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                    (0.06672455060314922 * C135 * (C157 + 1.) /
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                     1.) *
                    C440 *
                    (C47 * 0.3109069086965489e-1 * C110 *
                         (0.3109069086965489e-1 * (2 * C157 + 1) * 0.06672455060314922 * C135 -
                          0.06672455060314922 * (0.6218138173930979e-1 * C135 * (C157 + 1.) +
                                                 C135 * 0.3109069086965489e-1 * (2 * C157 + 1))) +
                     0. * C110 *
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                              0.06672455060314922 * C135 -
                          0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                              (2 * C157 + 1))) -
                C440 * C47 * 0.3109069086965489e-1 * C110 *
                    (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                         0.06672455060314922 * C135 -
                     0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                         (2 * C157 + 1)) *
                    (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                              0.06672455060314922 * C135 -
                          0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                              (2 * C157 + 1)) /
                         std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) +
                     0.6218138173930979e-1 * (2 * C157 + 1) * 0.3109069086965489e-1 *
                         ((C157 + std::pow(C157, 2)) + 1.) *
                         (0.06672455060314922 * C135 * (C157 + 1.) /
                              (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                          1.))) /
                   std::pow(std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                                (0.06672455060314922 * C135 * (C157 + 1.) /
                                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                 1.),
                            2)) +
              0.) *
                 C480) +
            ((((C47 * 0.3109069086965489e-1 *
                    std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.) +
                (0. * C47 +
                 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                               1.)) *
                    C417) +
               C424 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                    (0.06672455060314922 * C135 * (C157 + 1.) /
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                     1.))) +
              C440 *
                  (C47 * 0.3109069086965489e-1 * C110 *
                       (0. -
                        0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                   0. * C110 *
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                            0.06672455060314922 * C135 -
                        0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                            (2 * C157 + 1))) /
                  (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                   (0.06672455060314922 * C135 * (C157 + 1.) /
                        (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                    1.))) +
             0.) *
                C1122) +
           ((((0. * C47 +
               0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.)) *
                  C417 +
              (C424 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) +
               C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.)) /
                  (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                   (0.06672455060314922 * C135 * (C157 + 1.) /
                        (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                    1.))) +
             C440 *
                 (C47 * 0.3109069086965489e-1 * C110 *
                      (0. - 0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                  0. * C110 *
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                           0.06672455060314922 * C135 -
                       0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                           (2 * C157 + 1))) /
                 (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                  (0.06672455060314922 * C135 * (C157 + 1.) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                   1.))) +
            0.) *
               C1129) +
          ((((0. * C47 +
              0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                            1.)) *
                 C417 +
             C424 *
                 (C47 * 0.2074512376219157e-2 * (C157 + 1.) +
                  0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                  (0.06672455060314922 * C135 * (C157 + 1.) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                   1.))) +
            C440 *
                (C47 * 0.3109069086965489e-1 *
                     (C110 * (0. - 0.06672455060314922 *
                                       (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                           0.06672455060314922 * C135 -
                       0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                           (2 * C157 + 1))) +
                 0. * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1))) /
                (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                 (0.06672455060314922 * C135 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                  1.))) +
           0.) *
              C457) +
         ((((0. * C47 +
             0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.)) *
                C417 +
            C424 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                 (0.06672455060314922 * C135 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                  1.))) +
           (C440 *
                (C47 * 0.3109069086965489e-1 * C110 *
                     (0. - 0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                 0. * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1))) +
            C47 * 0.3109069086965489e-1 * C110 *
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) * 0.06672455060314922 *
                     C135 -
                 0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                     (2 * C157 + 1))) /
               (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                (0.06672455060314922 * C135 * (C157 + 1.) /
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                 1.))) +
          0.) *
             C1145) +
        0.;

    const double C1146 = C50 * (C50 - 1) * std::pow(C67, C50 - 2) * std::pow(C445, 2);
    const double C1147 = C51 * (C51 - 1) * std::pow(C67, C51 - 2) * std::pow(C445, 2);
    const double C1148 = C50 * (C50 - 1) * std::pow(C68, C50 - 2) * std::pow(C446, 2);
    const double C1149 = C51 * (C51 - 1) * std::pow(C68, C51 - 2) * std::pow(C446, 2);
    const double C1150 = C1146 + C1148;
    const double C1151 = C1147 + C1149;
    const double C1152 = std::sqrt(C73) * C1150;
    const double C1153 = C1151;
    const double C1154 = C1150 / 2.;
    const double C1155 = 2. * C1152;
    const double C1156 = 6 * C105 * std::pow(C455, 2) + 3 * std::pow(C105, 2) * C1154;
    const double C1157 = 0.3109069086965489e-1 * C1156 + 0.;
    const double C1158 = C47 * C1155;
    const double C1159 = C1158 / 2.;
    const double C1160 = C1159;
    const double C1161 = 2. * C1160;
    const double C1162 =
        (((-C442 * C124 / std::sqrt(C61)) / std::pow(2 * std::sqrt(C61) * C124, 2) -
          C462 / (2 * std::sqrt(C61) * std::pow(C124, 2))) *
             C442 +
         ((-C442 * 2 * std::sqrt(C61)) / std::pow(2 * std::sqrt(C61) * C124, 2) -
          (-C462 * std::sqrt(C61) * 2 * C124) / std::pow(C124, 4)) *
             C462) -
        C1161 * std::sqrt(C61) / std::pow(C124, 2);
    const double C1163 = 2 * std::pow(C463, 2) + 2 * C131 * C1162;
    const double C1164 = 0.0667245506031492 * C1163;
    const double C1165 = C137 * C1153;
    const double C1166 = C69 * C1165;
    const double C1167 = 0.0337738 * C1166;
    const double C1168 = C144 * C1153;
    const double C1169 = C64 * C1168;
    const double C1170 =
        (C1167 / C66 +
         ((-C468) / std::pow(C66, 2) - (-C444 * C145 * 2 * C66) / std::pow(C66, 4)) * C444) -
        C468 * C444 / std::pow(C66, 2);
    const double C1171 = C1170;
    const double C1172 = C1169 / C65;
    const double C1173 = C1171 + C1172;
    const double C1174 =
        ((C1173 / C111 +
          ((-C474) / std::pow(C111, 2) - (-C458 * C151 * 2 * C111) / std::pow(C111, 4)) * C458) -
         C1157 * C151 / std::pow(C111, 2)) -
        C474 * C458 / std::pow(C111, 2);
    const double C1175 = -C1174;
    const double C1176 = std::exp(C153) * std::pow(C476, 2) + std::exp(C153) * C1175;
    const double C1177 = C1176;
    const double C1178 = 0.3109069086965489e-1 * C1177 + 0.;
    const double C1179 =
        ((C1164 / C156 +
          ((-C465) / std::pow(C156, 2) - (-C479 * C136 * 2 * C156) / std::pow(C156, 4)) * C479) -
         C1178 * C136 / std::pow(C156, 2)) -
        C465 * C479 / std::pow(C156, 2);
    matrix.upper[14] =
        ((((((((((0. * C47 +
                  0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                    (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                1.)) *
                     C457 +
                 C464 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.))) +
                C480 *
                    (C47 * 0.3109069086965489e-1 * C110 *
                         (0. -
                          0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                     0. * C110 *
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                              0.06672455060314922 * C135 -
                          0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                              (2 * C157 + 1))) /
                    (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                     (0.06672455060314922 * C135 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      1.))) +
               ((((1. * C47 + (0. * C47 + 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                                            (0.3109069086965489e-1 *
                                                             ((C157 + std::pow(C157, 2)) + 1.)) +
                                                        1.)) *
                                  C457) +
                  C464 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.))) +
                 C480 *
                     (C47 * 0.3109069086965489e-1 * C110 *
                          (0. -
                           0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                      0. * C110 *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1))) /
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.))) +
                0.) *
                   C1173) +
              ((((C47 * 0.2074512376219157e-2 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                       (0.06672455060314922 * C135 * (C157 + 1.) /
                            (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                        1.)) +
                  0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                    (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                1.)) *
                     C457 +
                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.) *
                      C464 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) -
                  C464 * C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.) *
                      0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                      0.06672455060314922 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.))) /
                     std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                  (0.06672455060314922 * C135 * (C157 + 1.) /
                                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                   1.),
                              2)) +
                (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                     (0.06672455060314922 * C135 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      1.) *
                     C480 *
                     (C47 * 0.3109069086965489e-1 * C110 *
                          (0.2074512376219157e-2 * ((C157 + std::pow(C157, 2)) + 1.) -
                           0.06672455060314922 *
                               (0. * C135 * (C157 + 1.) +
                                (C157 + 1.) * 0.3109069086965489e-1 * (2 * C157 + 1))) +
                      0. * C110 *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1))) -
                 C480 * C47 * 0.3109069086965489e-1 * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1)) *
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                          0.06672455060314922 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      0. * ((C157 + std::pow(C157, 2)) + 1.) *
                          (0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.))) /
                    std::pow(
                        std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                            (0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.),
                        2)) +
               0.) *
                  C464) +
             ((((C47 * 0.3109069086965489e-1 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1)) /
                     (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                      (0.06672455060314922 * C135 * (C157 + 1.) /
                           (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                       1.)) +
                 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                               1.)) *
                    C457 +
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                     (0.06672455060314922 * C135 * (C157 + 1.) /
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                      1.) *
                     C464 *
                     (C47 * 0.2074512376219157e-2 * C110 +
                      0. * C110 * 0.06672455060314922 * (C157 + 1.)) -
                 C464 * C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.) *
                     0.3109069086965489e-1 *
                     (((C157 + std::pow(C157, 2)) + 1.) *
                          (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                               0.06672455060314922 * C135 -
                           0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                               (2 * C157 + 1)) /
                          std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) +
                      (2 * C157 + 1) *
                          (0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.))) /
                    std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                                 (0.06672455060314922 * C135 * (C157 + 1.) /
                                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                  1.),
                             2)) +
               (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                    (0.06672455060314922 * C135 * (C157 + 1.) /
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                     1.) *
                    C480 *
                    (C47 * 0.3109069086965489e-1 * C110 *
                         (0.3109069086965489e-1 * (2 * C157 + 1) * 0.06672455060314922 * C135 -
                          0.06672455060314922 * (0.6218138173930979e-1 * C135 * (C157 + 1.) +
                                                 C135 * 0.3109069086965489e-1 * (2 * C157 + 1))) +
                     0. * C110 *
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                              0.06672455060314922 * C135 -
                          0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                              (2 * C157 + 1))) -
                C480 * C47 * 0.3109069086965489e-1 * C110 *
                    (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                         0.06672455060314922 * C135 -
                     0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                         (2 * C157 + 1)) *
                    (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                              0.06672455060314922 * C135 -
                          0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                              (2 * C157 + 1)) /
                         std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) +
                     0.6218138173930979e-1 * (2 * C157 + 1) * 0.3109069086965489e-1 *
                         ((C157 + std::pow(C157, 2)) + 1.) *
                         (0.06672455060314922 * C135 * (C157 + 1.) /
                              (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                          1.))) /
                   std::pow(std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                                (0.06672455060314922 * C135 * (C157 + 1.) /
                                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                                 1.),
                            2)) +
              0.) *
                 C480) +
            ((((C47 * 0.3109069086965489e-1 *
                    std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.) +
                (0. * C47 +
                 0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                               1.)) *
                    C457) +
               C464 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                   (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                    (0.06672455060314922 * C135 * (C157 + 1.) /
                         (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                     1.))) +
              C480 *
                  (C47 * 0.3109069086965489e-1 * C110 *
                       (0. -
                        0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                   0. * C110 *
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                            0.06672455060314922 * C135 -
                        0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                            (2 * C157 + 1))) /
                  (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                   (0.06672455060314922 * C135 * (C157 + 1.) /
                        (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                    1.))) +
             0.) *
                C1156) +
           ((((0. * C47 +
               0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                             1.)) *
                  C457 +
              (C464 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) +
               C47 * 0.3109069086965489e-1 * C110 * 0.06672455060314922 * (C157 + 1.)) /
                  (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                   (0.06672455060314922 * C135 * (C157 + 1.) /
                        (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                    1.))) +
             C480 *
                 (C47 * 0.3109069086965489e-1 * C110 *
                      (0. - 0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                  0. * C110 *
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                           0.06672455060314922 * C135 -
                       0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                           (2 * C157 + 1))) /
                 (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                  (0.06672455060314922 * C135 * (C157 + 1.) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                   1.))) +
            0.) *
               C1163) +
          ((((0. * C47 +
              0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                            1.)) *
                 C457 +
             C464 *
                 (C47 * 0.2074512376219157e-2 * (C157 + 1.) +
                  0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                 (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                  (0.06672455060314922 * C135 * (C157 + 1.) /
                       (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                   1.))) +
            C480 *
                (C47 * 0.3109069086965489e-1 *
                     (C110 * (0. - 0.06672455060314922 *
                                       (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                           0.06672455060314922 * C135 -
                       0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                           (2 * C157 + 1))) +
                 0. * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1))) /
                (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                 (0.06672455060314922 * C135 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                  1.))) +
           0.) *
              C457) +
         ((((0. * C47 +
             0. * std::log(0.06672455060314922 * C135 * (C157 + 1.) /
                               (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                           1.)) *
                C457 +
            C464 * (0. * C47 + 0. * C110 * 0.06672455060314922 * (C157 + 1.)) /
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                 (0.06672455060314922 * C135 * (C157 + 1.) /
                      (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                  1.))) +
           (C480 *
                (C47 * 0.3109069086965489e-1 * C110 *
                     (0. - 0.06672455060314922 * (0. * C135 * (C157 + 1.) + 0. * (2 * C157 + 1))) +
                 0. * C110 *
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) *
                          0.06672455060314922 * C135 -
                      0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                          (2 * C157 + 1))) +
            C47 * 0.3109069086965489e-1 * C110 *
                (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.) * 0.06672455060314922 *
                     C135 -
                 0.06672455060314922 * C135 * (C157 + 1.) * 0.3109069086965489e-1 *
                     (2 * C157 + 1))) /
               (std::pow(0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.), 2) *
                (0.06672455060314922 * C135 * (C157 + 1.) /
                     (0.3109069086965489e-1 * ((C157 + std::pow(C157, 2)) + 1.)) +
                 1.))) +
          0.) *
             C1179) +
        0.;
}

} // namespace excgrid
