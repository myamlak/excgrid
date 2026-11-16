// excgrid codegen source: the Perdew 1986 GGA correlation functional, the gradient
// correction on the Perdew-Zunger LDA piece: J. P. Perdew, Phys. Rev. B 33, 8822 (1986).

// Auto-generated file, do not modify
#include "excgrid/kernel.hpp"

// The excgrid GGA PIECEWISE-kernel skeleton (fresh authorship; BSD-3-Clause).  A piecewise
// kernel is two branch bodies inside if/else, each with its own braces and CSE namespace;
// `cond` must select the branch the definition's first expression binds.
// The functional's SECOND-DERIVATIVE tier is emitted beside the kernel from the
// same two expressions, and carries the same branch: the matrix is one
// functional's, so a request must be answered by the branch the point is in and
// not by either branch alone.  It is the contract's upper triangle, row-major
// over the five components the kernel takes, in identifier order.
// The banner and the kernel.hpp include are emitted by the calling .ey, which
// always precedes this skeleton, so they are not repeated here.

#include <cmath>
#include <limits>

namespace excgrid {

XcKernelValue P86Correlation(
    double rhoA, double rhoB, double sigmaAa, double sigmaAb, double sigmaBb) {
    constexpr double eps = std::numeric_limits<double>::epsilon();
    [[maybe_unused]] constexpr double Pi = 3.14159265358979323846;
    // The same sigma-edge guard constant the straight-line skeleton declares.
    [[maybe_unused]] constexpr double SigmaGuard = 1e-300;

    XcKernelValue result;

    if (rhoA + rhoB < 2. * eps)
    {
        return result;
    }

    if (!(std::pow(3. / (4. * Pi * (rhoA + rhoB)), 1. / 3.) < 1.))
    {
        const double C1326 = 2. * sigmaAb;
        const double C1327 = rhoA + rhoB;
        const double C1328 = rhoA - rhoB;
        const double C1329 = 1. / 3.;
        const double C1330 = 2. / 3.;
        const double C1331 = 4. / 3.;
        const double C1332 = 5. / 3.;
        const double C1333 = 0.9999999999999999 * C1328;
        const double C1334 = Pi * C1327;
        const double C1335 = sigmaAa + C1326;
        const double C1336 = 4. * C1334;
        const double C1337 = C1335 + sigmaBb;
        const double C1338 = C1333 / C1327;
        const double C1339 = C1338 + 1.;
        const double C1340 = 1. - C1338;
        const double C1341 = 0.22167 / C1336;
        const double C1342 = 3. / C1336;
        const double C1343 = std::pow(C1342, C1329);
        const double C1344 = std::pow(C1342, C1330);
        const double C1345 = 7.389e-6 * C1344;
        const double C1346 = 0.023266 * C1343;
        const double C1347 = 0.3334 * C1343;
        const double C1348 = 0.472 * C1344;
        const double C1349 = 8.723 * C1343;
        const double C1350 = std::sqrt(C1343);
        const double C1351 = 1.0529 * C1350;
        const double C1352 = C1346 + C1345;
        const double C1353 = C1349 + C1348;
        const double C1354 = C1351 + C1347;
        const double C1355 = C1352 + 0.002568;
        const double C1356 = C1353 + C1341;
        const double C1357 = C1354 + 1.;
        const double C1358 = C1356 + 1.;
        const double C1359 = 0.1423 / C1357;
        const double C1360 = C1355 / C1358;
        const double C1361 = C1360 + 0.001667;

        result.exc =
            C1327 *
            (((C1359 - 0.0843 / ((1.3981 * C1350 + 0.2611 * C1343) + 1.)) *
                  ((std::pow(C1339, C1331) + std::pow(C1340, C1331)) - 2.) /
                  (2. * (std::pow(2., C1329) - 1.)) -
              C1359) +
             std::exp(-0.00081290825 * std::sqrt(C1337) / (std::pow(C1327, 7. / 6.) * C1361)) *
                 C1361 * C1337 /
                 (std::pow(C1327, 7. / 3.) *
                  std::sqrt((std::pow(C1339, C1332) + std::pow(C1340, C1332)) / 2.)));

        const double C1363 = -24. * Pi;
        const double C1364 = 0.2611 * C1343;
        const double C1365 = 0.88668 * Pi;
        const double C1366 = 0.9999999999999999 * C1327;
        const double C1367 = 1.3981 * C1350;
        const double C1368 = 2 * C1350;
        const double C1369 = 12. * Pi;
        const double C1370 = -2. / 3.;
        const double C1371 = -1. / 3.;
        const double C1372 = 7. / 3.;
        const double C1373 = 7. / 6.;
        const double C1374 = std::sqrt(C1337);
        const double C1375 = std::pow(2., C1329);
        const double C1376 = std::pow(C1327, 2);
        const double C1377 = std::pow(C1336, 2);
        const double C1378 = std::pow(C1339, C1331);
        const double C1379 = std::pow(C1339, C1332);
        const double C1380 = std::pow(C1340, C1331);
        const double C1381 = std::pow(C1340, C1332);
        const double C1382 = std::pow(C1357, 2);
        const double C1383 = std::pow(C1358, 2);
        const double C1384 = 0.00081290825 * C1374;
        const double C1385 = 3. * C1377;
        const double C1386 = C1377 * C1368;
        const double C1387 = C1367 + C1364;
        const double C1388 = C1378 + C1380;
        const double C1389 = C1379 + C1381;
        const double C1390 = C1366 - C1333;
        const double C1391 = C1375 - 1.;
        const double C1392 = C1365 / C1377;
        const double C1393 = std::pow(C1327, C1372);
        const double C1394 = std::pow(C1327, C1373);
        const double C1395 = std::pow(C1342, C1370);
        const double C1396 = std::pow(C1342, C1371);
        const double C1397 = 2. * C1391;
        const double C1398 = 3. * C1386;
        const double C1399 = C1394 * C1361;
        const double C1400 = C1395 * C1369;
        const double C1401 = C1396 * C1363;
        const double C1402 = C1387 + 1.;
        const double C1403 = C1388 - 2.;
        const double C1404 = C1389 / 2.;
        const double C1405 = -8.723 * C1400;
        const double C1406 = -1.0529 * C1400;
        const double C1407 = -0.3334 * C1400;
        const double C1408 = -0.023266 * C1400;
        const double C1409 = 7.389e-6 * C1401;
        const double C1410 = 0.472 * C1401;
        const double C1411 = 0.0843 / C1402;
        const double C1412 = C1384 / C1399;
        const double C1413 = std::sqrt(C1404);
        const double C1414 = C1393 * C1413;
        const double C1415 = C1405 + C1410;
        const double C1416 = C1408 + C1409;
        const double C1417 = C1359 - C1411;
        const double C1418 = -C1412;
        const double C1419 = C1406 / C1398;
        const double C1420 = C1407 / C1385;
        const double C1421 = C1358 * C1416;
        const double C1422 = C1419 + C1420;
        const double C1423 = C1415 / C1385;
        const double C1424 = std::exp(C1418);
        const double C1425 = 0.1423 * C1422;
        const double C1426 = C1424 * C1361;
        const double C1427 = C1423 - C1392;
        const double C1428 = C1421 / C1385;
        const double C1429 = C1355 * C1427;
        const double C1430 = C1426 * C1337;
        const double C1431 = -C1425;
        const double C1432 = C1428 - C1429;
        const double C1433 = C1431 / C1382;

        result.vrhoA =
            C1327 *
                (((C1417 *
                       (std::pow(C1339, C1329) * 4. * C1390 +
                        std::pow(C1340, C1329) * -4. * C1390) /
                       (3. * C1376) +
                   (C1433 - (-0.0843 * (-1.3981 * C1400 / C1398 + -0.2611 * C1400 / C1385)) /
                                std::pow(C1402, 2)) *
                       C1403) /
                      C1397 -
                  C1433) +
                 (C1414 *
                      (C1424 * C1432 / C1383 +
                       C1361 * C1424 * 0.00081290825 * C1374 *
                           (C1394 * C1432 / C1383 + C1361 * 7. * std::pow(C1327, 1. / 6.) / 6.) /
                           std::pow(C1399, 2)) *
                      C1337 -
                  C1430 * (C1393 *
                               (std::pow(C1339, C1330) * 5. * C1390 +
                                std::pow(C1340, C1330) * -5. * C1390) /
                               (6. * C1376 * 2 * C1413) +
                           C1413 * 7. * std::pow(C1327, C1331) / 3.)) /
                     std::pow(C1414, 2)) +
            ((C1417 * C1403 / C1397 - C1359) + C1430 / C1414);

        const double C1435 = -24. * Pi;
        const double C1436 = -0.9999999999999999 * C1327;
        const double C1437 = 0.2611 * C1343;
        const double C1438 = 0.88668 * Pi;
        const double C1439 = 1.3981 * C1350;
        const double C1440 = 2 * C1350;
        const double C1441 = 12. * Pi;
        const double C1442 = -2. / 3.;
        const double C1443 = -1. / 3.;
        const double C1444 = 7. / 3.;
        const double C1445 = 7. / 6.;
        const double C1446 = std::sqrt(C1337);
        const double C1447 = std::pow(2., C1329);
        const double C1448 = std::pow(C1327, 2);
        const double C1449 = std::pow(C1336, 2);
        const double C1450 = std::pow(C1339, C1331);
        const double C1451 = std::pow(C1339, C1332);
        const double C1452 = std::pow(C1340, C1331);
        const double C1453 = std::pow(C1340, C1332);
        const double C1454 = std::pow(C1357, 2);
        const double C1455 = std::pow(C1358, 2);
        const double C1456 = 0.00081290825 * C1446;
        const double C1457 = 3. * C1449;
        const double C1458 = C1449 * C1440;
        const double C1459 = C1439 + C1437;
        const double C1460 = C1450 + C1452;
        const double C1461 = C1451 + C1453;
        const double C1462 = C1436 - C1333;
        const double C1463 = C1447 - 1.;
        const double C1464 = C1438 / C1449;
        const double C1465 = std::pow(C1327, C1444);
        const double C1466 = std::pow(C1327, C1445);
        const double C1467 = std::pow(C1342, C1442);
        const double C1468 = std::pow(C1342, C1443);
        const double C1469 = 2. * C1463;
        const double C1470 = 3. * C1458;
        const double C1471 = C1466 * C1361;
        const double C1472 = C1467 * C1441;
        const double C1473 = C1468 * C1435;
        const double C1474 = C1459 + 1.;
        const double C1475 = C1460 - 2.;
        const double C1476 = C1461 / 2.;
        const double C1477 = -8.723 * C1472;
        const double C1478 = -1.0529 * C1472;
        const double C1479 = -0.3334 * C1472;
        const double C1480 = -0.023266 * C1472;
        const double C1481 = 7.389e-6 * C1473;
        const double C1482 = 0.472 * C1473;
        const double C1483 = 0.0843 / C1474;
        const double C1484 = C1456 / C1471;
        const double C1485 = std::sqrt(C1476);
        const double C1486 = C1465 * C1485;
        const double C1487 = C1477 + C1482;
        const double C1488 = C1480 + C1481;
        const double C1489 = C1359 - C1483;
        const double C1490 = -C1484;
        const double C1491 = C1478 / C1470;
        const double C1492 = C1479 / C1457;
        const double C1493 = C1358 * C1488;
        const double C1494 = C1491 + C1492;
        const double C1495 = C1487 / C1457;
        const double C1496 = std::exp(C1490);
        const double C1497 = 0.1423 * C1494;
        const double C1498 = C1496 * C1361;
        const double C1499 = C1495 - C1464;
        const double C1500 = C1493 / C1457;
        const double C1501 = C1355 * C1499;
        const double C1502 = C1498 * C1337;
        const double C1503 = -C1497;
        const double C1504 = C1500 - C1501;
        const double C1505 = C1503 / C1454;

        result.vrhoB =
            C1327 *
                (((C1489 *
                       (std::pow(C1339, C1329) * 4. * C1462 +
                        std::pow(C1340, C1329) * -4. * C1462) /
                       (3. * C1448) +
                   (C1505 - (-0.0843 * (-1.3981 * C1472 / C1470 + -0.2611 * C1472 / C1457)) /
                                std::pow(C1474, 2)) *
                       C1475) /
                      C1469 -
                  C1505) +
                 (C1486 *
                      (C1496 * C1504 / C1455 +
                       C1361 * C1496 * 0.00081290825 * C1446 *
                           (C1466 * C1504 / C1455 + C1361 * 7. * std::pow(C1327, 1. / 6.) / 6.) /
                           std::pow(C1471, 2)) *
                      C1337 -
                  C1502 * (C1465 *
                               (std::pow(C1339, C1330) * 5. * C1462 +
                                std::pow(C1340, C1330) * -5. * C1462) /
                               (6. * C1448 * 2 * C1485) +
                           C1485 * 7. * std::pow(C1327, C1331) / 3.)) /
                     std::pow(C1486, 2)) +
            ((C1489 * C1475 / C1469 - C1359) + C1502 / C1486);

        const double C1507 = 2. * sigmaAb;
        const double C1508 = rhoA + rhoB;
        const double C1509 = rhoA - rhoB;
        const double C1510 = 1. / 3.;
        const double C1511 = 2. / 3.;
        const double C1512 = 5. / 3.;
        const double C1513 = 7. / 6.;
        const double C1514 = 0.9999999999999999 * C1509;
        const double C1515 = Pi * C1508;
        const double C1516 = sigmaAa + C1507;
        const double C1517 = std::pow(C1508, C1513);
        const double C1518 = 4. * C1515;
        const double C1519 = C1516 + sigmaBb;
        const double C1520 = C1514 / C1508;
        const double C1521 = C1519 + SigmaGuard;
        const double C1522 = C1520 + 1.;
        const double C1523 = 1. - C1520;
        const double C1524 = 0.22167 / C1518;
        const double C1525 = 3. / C1518;
        const double C1526 = std::sqrt(C1521);
        const double C1527 = std::pow(C1525, C1510);
        const double C1528 = std::pow(C1525, C1511);
        const double C1529 = 7.389e-6 * C1528;
        const double C1530 = 0.00081290825 * C1526;
        const double C1531 = 0.023266 * C1527;
        const double C1532 = 0.472 * C1528;
        const double C1533 = 8.723 * C1527;
        const double C1534 = std::sqrt(C1527);
        const double C1535 = C1531 + C1529;
        const double C1536 = C1533 + C1532;
        const double C1537 = C1535 + 0.002568;
        const double C1538 = C1536 + C1524;
        const double C1539 = C1538 + 1.;
        const double C1540 = C1537 / C1539;
        const double C1541 = C1540 + 0.001667;
        const double C1542 = C1517 * C1541;
        const double C1543 = C1530 / C1542;
        const double C1544 = -C1543;
        const double C1545 = std::exp(C1544);

        result.vsigmaAa =
            C1508 *
            ((0.1423 / ((1.0529 * C1534 + 0.3334 * C1527) + 1.) -
              0.0843 / ((1.3981 * C1534 + 0.2611 * C1527) + 1.)) *
                 (0. * std::pow(C1522, C1510) + 0. * std::pow(C1523, C1510)) /
                 (3. * C1508 * 2. * (std::pow(2., C1510) - 1.)) +
             (C1545 * C1541 +
              (0. * C1545 / C1539 + C1541 * -0.00081290825 * C1545 / (2 * C1526 * C1542)) * C1519) /
                 (std::pow(C1508, 7. / 3.) *
                  std::sqrt((std::pow(C1522, C1512) + std::pow(C1523, C1512)) / 2.)));

        const double C1547 = 2. * SigmaGuard;
        const double C1548 = 2. * sigmaAb;
        const double C1549 = rhoA + rhoB;
        const double C1550 = rhoA - rhoB;
        const double C1551 = 1. / 3.;
        const double C1552 = 2. / 3.;
        const double C1553 = 5. / 3.;
        const double C1554 = 7. / 6.;
        const double C1555 = 0.9999999999999999 * C1550;
        const double C1556 = Pi * C1549;
        const double C1557 = sigmaAa + C1548;
        const double C1558 = std::pow(C1549, C1554);
        const double C1559 = 4. * C1556;
        const double C1560 = C1557 + sigmaBb;
        const double C1561 = C1555 / C1549;
        const double C1562 = C1560 + C1547;
        const double C1563 = C1561 + 1.;
        const double C1564 = 1. - C1561;
        const double C1565 = 0.22167 / C1559;
        const double C1566 = 3. / C1559;
        const double C1567 = std::sqrt(C1562);
        const double C1568 = std::pow(C1566, C1551);
        const double C1569 = std::pow(C1566, C1552);
        const double C1570 = 7.389e-6 * C1569;
        const double C1571 = 0.00081290825 * C1567;
        const double C1572 = 0.023266 * C1568;
        const double C1573 = 0.472 * C1569;
        const double C1574 = 8.723 * C1568;
        const double C1575 = std::sqrt(C1568);
        const double C1576 = C1572 + C1570;
        const double C1577 = C1574 + C1573;
        const double C1578 = C1576 + 0.002568;
        const double C1579 = C1577 + C1565;
        const double C1580 = C1579 + 1.;
        const double C1581 = C1578 / C1580;
        const double C1582 = C1581 + 0.001667;
        const double C1583 = C1558 * C1582;
        const double C1584 = C1571 / C1583;
        const double C1585 = -C1584;
        const double C1586 = std::exp(C1585);

        result.vsigmaAb =
            C1549 *
            ((0.1423 / ((1.0529 * C1575 + 0.3334 * C1568) + 1.) -
              0.0843 / ((1.3981 * C1575 + 0.2611 * C1568) + 1.)) *
                 (0. * std::pow(C1563, C1551) + 0. * std::pow(C1564, C1551)) /
                 (3. * C1549 * 2. * (std::pow(2., C1551) - 1.)) +
             (2. * C1586 * C1582 +
              (0. * C1586 / C1580 + C1582 * -0.0016258165 * C1586 / (2 * C1567 * C1583)) * C1560) /
                 (std::pow(C1549, 7. / 3.) *
                  std::sqrt((std::pow(C1563, C1553) + std::pow(C1564, C1553)) / 2.)));

        const double C1588 = 2. * sigmaAb;
        const double C1589 = rhoA + rhoB;
        const double C1590 = rhoA - rhoB;
        const double C1591 = 1. / 3.;
        const double C1592 = 2. / 3.;
        const double C1593 = 5. / 3.;
        const double C1594 = 7. / 6.;
        const double C1595 = 0.9999999999999999 * C1590;
        const double C1596 = Pi * C1589;
        const double C1597 = sigmaAa + C1588;
        const double C1598 = std::pow(C1589, C1594);
        const double C1599 = 4. * C1596;
        const double C1600 = C1597 + sigmaBb;
        const double C1601 = C1595 / C1589;
        const double C1602 = C1600 + SigmaGuard;
        const double C1603 = C1601 + 1.;
        const double C1604 = 1. - C1601;
        const double C1605 = 0.22167 / C1599;
        const double C1606 = 3. / C1599;
        const double C1607 = std::sqrt(C1602);
        const double C1608 = std::pow(C1606, C1591);
        const double C1609 = std::pow(C1606, C1592);
        const double C1610 = 7.389e-6 * C1609;
        const double C1611 = 0.00081290825 * C1607;
        const double C1612 = 0.023266 * C1608;
        const double C1613 = 0.472 * C1609;
        const double C1614 = 8.723 * C1608;
        const double C1615 = std::sqrt(C1608);
        const double C1616 = C1612 + C1610;
        const double C1617 = C1614 + C1613;
        const double C1618 = C1616 + 0.002568;
        const double C1619 = C1617 + C1605;
        const double C1620 = C1619 + 1.;
        const double C1621 = C1618 / C1620;
        const double C1622 = C1621 + 0.001667;
        const double C1623 = C1598 * C1622;
        const double C1624 = C1611 / C1623;
        const double C1625 = -C1624;
        const double C1626 = std::exp(C1625);

        result.vsigmaBb =
            C1589 *
            ((0.1423 / ((1.0529 * C1615 + 0.3334 * C1608) + 1.) -
              0.0843 / ((1.3981 * C1615 + 0.2611 * C1608) + 1.)) *
                 (0. * std::pow(C1603, C1591) + 0. * std::pow(C1604, C1591)) /
                 (3. * C1589 * 2. * (std::pow(2., C1591) - 1.)) +
             (C1626 * C1622 +
              (0. * C1626 / C1620 + C1622 * -0.00081290825 * C1626 / (2 * C1607 * C1623)) * C1600) /
                 (std::pow(C1589, 7. / 3.) *
                  std::sqrt((std::pow(C1603, C1593) + std::pow(C1604, C1593)) / 2.)));
    } else
    {
        const double C1628 = 2. * sigmaAb;
        const double C1629 = rhoA + rhoB;
        const double C1630 = rhoA - rhoB;
        const double C1631 = 1. / 3.;
        const double C1632 = 2. / 3.;
        const double C1633 = 4. / 3.;
        const double C1634 = 5. / 3.;
        const double C1635 = 0.9999999999999999 * C1630;
        const double C1636 = Pi * C1629;
        const double C1637 = sigmaAa + C1628;
        const double C1638 = 4. * C1636;
        const double C1639 = C1637 + sigmaBb;
        const double C1640 = C1635 / C1629;
        const double C1641 = C1640 + 1.;
        const double C1642 = 1. - C1640;
        const double C1643 = 0.22167 / C1638;
        const double C1644 = 3. / C1638;
        const double C1645 = std::pow(C1644, C1631);
        const double C1646 = std::pow(C1644, C1632);
        const double C1647 = 7.389e-6 * C1646;
        const double C1648 = 0.0116 * C1645;
        const double C1649 = 0.023266 * C1645;
        const double C1650 = 0.472 * C1646;
        const double C1651 = 8.723 * C1645;
        const double C1652 = std::log(C1645);
        const double C1653 = 0.0311 * C1652;
        const double C1654 = C1645 * C1652;
        const double C1655 = C1649 + C1647;
        const double C1656 = C1651 + C1650;
        const double C1657 = 0.0020 * C1654;
        const double C1658 = C1655 + 0.002568;
        const double C1659 = C1656 + C1643;
        const double C1660 = C1653 + C1657;
        const double C1661 = C1659 + 1.;
        const double C1662 = C1660 - 0.048;
        const double C1663 = C1658 / C1661;
        const double C1664 = C1663 + 0.001667;
        const double C1665 = C1662 - C1648;

        result.exc =
            C1629 *
            ((C1665 + ((((0.01555 * C1652 + 0.0007 * C1654) - 0.0269) - 0.0048 * C1645) - C1665) *
                          ((std::pow(C1641, C1633) + std::pow(C1642, C1633)) - 2.) /
                          (2. * (std::pow(2., C1631) - 1.))) +
             std::exp(-0.00081290825 * std::sqrt(C1639) / (std::pow(C1629, 7. / 6.) * C1664)) *
                 C1664 * C1639 /
                 (std::pow(C1629, 7. / 3.) *
                  std::sqrt((std::pow(C1641, C1634) + std::pow(C1642, C1634)) / 2.)));

        const double C1667 = -24. * Pi;
        const double C1668 = 0.0007 * C1654;
        const double C1669 = 0.0048 * C1645;
        const double C1670 = 0.01555 * C1652;
        const double C1671 = 0.88668 * Pi;
        const double C1672 = 0.9999999999999999 * C1629;
        const double C1673 = 12. * Pi;
        const double C1674 = -2. / 3.;
        const double C1675 = -1. / 3.;
        const double C1676 = 7. / 3.;
        const double C1677 = 7. / 6.;
        const double C1678 = std::sqrt(C1639);
        const double C1679 = std::pow(2., C1631);
        const double C1680 = std::pow(C1629, 2);
        const double C1681 = std::pow(C1638, 2);
        const double C1682 = std::pow(C1641, C1633);
        const double C1683 = std::pow(C1641, C1634);
        const double C1684 = std::pow(C1642, C1633);
        const double C1685 = std::pow(C1642, C1634);
        const double C1686 = std::pow(C1661, 2);
        const double C1687 = 0.00081290825 * C1678;
        const double C1688 = 3. * C1681;
        const double C1689 = C1681 * C1645;
        const double C1690 = C1670 + C1668;
        const double C1691 = C1682 + C1684;
        const double C1692 = C1683 + C1685;
        const double C1693 = C1672 - C1635;
        const double C1694 = C1679 - 1.;
        const double C1695 = C1671 / C1681;
        const double C1696 = std::pow(C1629, C1676);
        const double C1697 = std::pow(C1629, C1677);
        const double C1698 = std::pow(C1644, C1674);
        const double C1699 = std::pow(C1644, C1675);
        const double C1700 = 2. * C1694;
        const double C1701 = 3. * C1689;
        const double C1702 = C1697 * C1664;
        const double C1703 = C1698 * C1673;
        const double C1704 = C1699 * C1667;
        const double C1705 = C1690 - 0.0269;
        const double C1706 = C1691 - 2.;
        const double C1707 = C1692 / 2.;
        const double C1708 = -8.723 * C1703;
        const double C1709 = -0.0311 * C1703;
        const double C1710 = -0.023266 * C1703;
        const double C1711 = -0.0116 * C1703;
        const double C1712 = 7.389e-6 * C1704;
        const double C1713 = 0.472 * C1704;
        const double C1714 = C1645 * C1703;
        const double C1715 = C1652 * C1703;
        const double C1716 = C1705 - C1669;
        const double C1717 = C1687 / C1702;
        const double C1718 = std::sqrt(C1707);
        const double C1719 = C1696 * C1718;
        const double C1720 = C1708 + C1713;
        const double C1721 = C1710 + C1712;
        const double C1722 = -C1715;
        const double C1723 = C1716 - C1665;
        const double C1724 = -C1717;
        const double C1725 = C1709 / C1701;
        const double C1726 = C1711 / C1688;
        const double C1727 = C1714 / C1701;
        const double C1728 = C1661 * C1721;
        const double C1729 = C1720 / C1688;
        const double C1730 = C1722 / C1688;
        const double C1731 = std::exp(C1724);
        const double C1732 = C1731 * C1664;
        const double C1733 = C1729 - C1695;
        const double C1734 = C1730 - C1727;
        const double C1735 = C1728 / C1688;
        const double C1736 = 0.0020 * C1734;
        const double C1737 = C1658 * C1733;
        const double C1738 = C1732 * C1639;
        const double C1739 = C1725 + C1736;
        const double C1740 = C1735 - C1737;
        const double C1741 = C1739 - C1726;

        result.vrhoA =
            C1629 *
                ((C1741 +
                  (C1723 *
                       (std::pow(C1641, C1631) * 4. * C1693 +
                        std::pow(C1642, C1631) * -4. * C1693) /
                       (3. * C1680) +
                   (((-0.01555 * C1703 / C1701 + 0.0007 * C1734) - -0.0048 * C1703 / C1688) -
                    C1741) *
                       C1706) /
                      C1700) +
                 (C1719 *
                      (C1731 * C1740 / C1686 +
                       C1664 * C1731 * 0.00081290825 * C1678 *
                           (C1697 * C1740 / C1686 + C1664 * 7. * std::pow(C1629, 1. / 6.) / 6.) /
                           std::pow(C1702, 2)) *
                      C1639 -
                  C1738 * (C1696 *
                               (std::pow(C1641, C1632) * 5. * C1693 +
                                std::pow(C1642, C1632) * -5. * C1693) /
                               (6. * C1680 * 2 * C1718) +
                           C1718 * 7. * std::pow(C1629, C1633) / 3.)) /
                     std::pow(C1719, 2)) +
            ((C1665 + C1723 * C1706 / C1700) + C1738 / C1719);

        const double C1743 = -24. * Pi;
        const double C1744 = -0.9999999999999999 * C1629;
        const double C1745 = 0.0007 * C1654;
        const double C1746 = 0.0048 * C1645;
        const double C1747 = 0.01555 * C1652;
        const double C1748 = 0.88668 * Pi;
        const double C1749 = 12. * Pi;
        const double C1750 = -2. / 3.;
        const double C1751 = -1. / 3.;
        const double C1752 = 7. / 3.;
        const double C1753 = 7. / 6.;
        const double C1754 = std::sqrt(C1639);
        const double C1755 = std::pow(2., C1631);
        const double C1756 = std::pow(C1629, 2);
        const double C1757 = std::pow(C1638, 2);
        const double C1758 = std::pow(C1641, C1633);
        const double C1759 = std::pow(C1641, C1634);
        const double C1760 = std::pow(C1642, C1633);
        const double C1761 = std::pow(C1642, C1634);
        const double C1762 = std::pow(C1661, 2);
        const double C1763 = 0.00081290825 * C1754;
        const double C1764 = 3. * C1757;
        const double C1765 = C1757 * C1645;
        const double C1766 = C1747 + C1745;
        const double C1767 = C1758 + C1760;
        const double C1768 = C1759 + C1761;
        const double C1769 = C1744 - C1635;
        const double C1770 = C1755 - 1.;
        const double C1771 = C1748 / C1757;
        const double C1772 = std::pow(C1629, C1752);
        const double C1773 = std::pow(C1629, C1753);
        const double C1774 = std::pow(C1644, C1750);
        const double C1775 = std::pow(C1644, C1751);
        const double C1776 = 2. * C1770;
        const double C1777 = 3. * C1765;
        const double C1778 = C1773 * C1664;
        const double C1779 = C1774 * C1749;
        const double C1780 = C1775 * C1743;
        const double C1781 = C1766 - 0.0269;
        const double C1782 = C1767 - 2.;
        const double C1783 = C1768 / 2.;
        const double C1784 = -8.723 * C1779;
        const double C1785 = -0.0311 * C1779;
        const double C1786 = -0.023266 * C1779;
        const double C1787 = -0.0116 * C1779;
        const double C1788 = 7.389e-6 * C1780;
        const double C1789 = 0.472 * C1780;
        const double C1790 = C1645 * C1779;
        const double C1791 = C1652 * C1779;
        const double C1792 = C1781 - C1746;
        const double C1793 = C1763 / C1778;
        const double C1794 = std::sqrt(C1783);
        const double C1795 = C1772 * C1794;
        const double C1796 = C1784 + C1789;
        const double C1797 = C1786 + C1788;
        const double C1798 = -C1791;
        const double C1799 = C1792 - C1665;
        const double C1800 = -C1793;
        const double C1801 = C1785 / C1777;
        const double C1802 = C1787 / C1764;
        const double C1803 = C1790 / C1777;
        const double C1804 = C1661 * C1797;
        const double C1805 = C1796 / C1764;
        const double C1806 = C1798 / C1764;
        const double C1807 = std::exp(C1800);
        const double C1808 = C1807 * C1664;
        const double C1809 = C1805 - C1771;
        const double C1810 = C1806 - C1803;
        const double C1811 = C1804 / C1764;
        const double C1812 = 0.0020 * C1810;
        const double C1813 = C1658 * C1809;
        const double C1814 = C1808 * C1639;
        const double C1815 = C1801 + C1812;
        const double C1816 = C1811 - C1813;
        const double C1817 = C1815 - C1802;

        result.vrhoB =
            C1629 *
                ((C1817 +
                  (C1799 *
                       (std::pow(C1641, C1631) * 4. * C1769 +
                        std::pow(C1642, C1631) * -4. * C1769) /
                       (3. * C1756) +
                   (((-0.01555 * C1779 / C1777 + 0.0007 * C1810) - -0.0048 * C1779 / C1764) -
                    C1817) *
                       C1782) /
                      C1776) +
                 (C1795 *
                      (C1807 * C1816 / C1762 +
                       C1664 * C1807 * 0.00081290825 * C1754 *
                           (C1773 * C1816 / C1762 + C1664 * 7. * std::pow(C1629, 1. / 6.) / 6.) /
                           std::pow(C1778, 2)) *
                      C1639 -
                  C1814 * (C1772 *
                               (std::pow(C1641, C1632) * 5. * C1769 +
                                std::pow(C1642, C1632) * -5. * C1769) /
                               (6. * C1756 * 2 * C1794) +
                           C1794 * 7. * std::pow(C1629, C1633) / 3.)) /
                     std::pow(C1795, 2)) +
            ((C1665 + C1799 * C1782 / C1776) + C1814 / C1795);

        const double C1819 = 2. * sigmaAb;
        const double C1820 = rhoA + rhoB;
        const double C1821 = rhoA - rhoB;
        const double C1822 = 1. / 3.;
        const double C1823 = 2. / 3.;
        const double C1824 = 5. / 3.;
        const double C1825 = 7. / 6.;
        const double C1826 = 0.9999999999999999 * C1821;
        const double C1827 = Pi * C1820;
        const double C1828 = sigmaAa + C1819;
        const double C1829 = std::pow(C1820, C1825);
        const double C1830 = 4. * C1827;
        const double C1831 = C1828 + sigmaBb;
        const double C1832 = C1826 / C1820;
        const double C1833 = C1831 + SigmaGuard;
        const double C1834 = C1832 + 1.;
        const double C1835 = 1. - C1832;
        const double C1836 = 0.22167 / C1830;
        const double C1837 = 3. / C1830;
        const double C1838 = std::sqrt(C1833);
        const double C1839 = std::pow(C1837, C1822);
        const double C1840 = std::pow(C1837, C1823);
        const double C1841 = 7.389e-6 * C1840;
        const double C1842 = 0.00081290825 * C1838;
        const double C1843 = 0.023266 * C1839;
        const double C1844 = 0.472 * C1840;
        const double C1845 = 8.723 * C1839;
        const double C1846 = std::log(C1839);
        const double C1847 = C1839 * C1846;
        const double C1848 = C1843 + C1841;
        const double C1849 = C1845 + C1844;
        const double C1850 = C1848 + 0.002568;
        const double C1851 = C1849 + C1836;
        const double C1852 = C1851 + 1.;
        const double C1853 = C1850 / C1852;
        const double C1854 = C1853 + 0.001667;
        const double C1855 = C1829 * C1854;
        const double C1856 = C1842 / C1855;
        const double C1857 = -C1856;
        const double C1858 = std::exp(C1857);

        result.vsigmaAa =
            C1820 *
            (((((0.01555 * C1846 + 0.0007 * C1847) - 0.0269) - 0.0048 * C1839) -
              (((0.0311 * C1846 + 0.0020 * C1847) - 0.048) - 0.0116 * C1839)) *
                 (0. * std::pow(C1834, C1822) + 0. * std::pow(C1835, C1822)) /
                 (3. * C1820 * 2. * (std::pow(2., C1822) - 1.)) +
             (C1858 * C1854 +
              (0. * C1858 / C1852 + C1854 * -0.00081290825 * C1858 / (2 * C1838 * C1855)) * C1831) /
                 (std::pow(C1820, 7. / 3.) *
                  std::sqrt((std::pow(C1834, C1824) + std::pow(C1835, C1824)) / 2.)));

        const double C1860 = 2. * SigmaGuard;
        const double C1861 = 2. * sigmaAb;
        const double C1862 = rhoA + rhoB;
        const double C1863 = rhoA - rhoB;
        const double C1864 = 1. / 3.;
        const double C1865 = 2. / 3.;
        const double C1866 = 5. / 3.;
        const double C1867 = 7. / 6.;
        const double C1868 = 0.9999999999999999 * C1863;
        const double C1869 = Pi * C1862;
        const double C1870 = sigmaAa + C1861;
        const double C1871 = std::pow(C1862, C1867);
        const double C1872 = 4. * C1869;
        const double C1873 = C1870 + sigmaBb;
        const double C1874 = C1868 / C1862;
        const double C1875 = C1873 + C1860;
        const double C1876 = C1874 + 1.;
        const double C1877 = 1. - C1874;
        const double C1878 = 0.22167 / C1872;
        const double C1879 = 3. / C1872;
        const double C1880 = std::sqrt(C1875);
        const double C1881 = std::pow(C1879, C1864);
        const double C1882 = std::pow(C1879, C1865);
        const double C1883 = 7.389e-6 * C1882;
        const double C1884 = 0.00081290825 * C1880;
        const double C1885 = 0.023266 * C1881;
        const double C1886 = 0.472 * C1882;
        const double C1887 = 8.723 * C1881;
        const double C1888 = std::log(C1881);
        const double C1889 = C1881 * C1888;
        const double C1890 = C1885 + C1883;
        const double C1891 = C1887 + C1886;
        const double C1892 = C1890 + 0.002568;
        const double C1893 = C1891 + C1878;
        const double C1894 = C1893 + 1.;
        const double C1895 = C1892 / C1894;
        const double C1896 = C1895 + 0.001667;
        const double C1897 = C1871 * C1896;
        const double C1898 = C1884 / C1897;
        const double C1899 = -C1898;
        const double C1900 = std::exp(C1899);

        result.vsigmaAb =
            C1862 *
            (((((0.01555 * C1888 + 0.0007 * C1889) - 0.0269) - 0.0048 * C1881) -
              (((0.0311 * C1888 + 0.0020 * C1889) - 0.048) - 0.0116 * C1881)) *
                 (0. * std::pow(C1876, C1864) + 0. * std::pow(C1877, C1864)) /
                 (3. * C1862 * 2. * (std::pow(2., C1864) - 1.)) +
             (2. * C1900 * C1896 +
              (0. * C1900 / C1894 + C1896 * -0.0016258165 * C1900 / (2 * C1880 * C1897)) * C1873) /
                 (std::pow(C1862, 7. / 3.) *
                  std::sqrt((std::pow(C1876, C1866) + std::pow(C1877, C1866)) / 2.)));

        const double C1902 = 2. * sigmaAb;
        const double C1903 = rhoA + rhoB;
        const double C1904 = rhoA - rhoB;
        const double C1905 = 1. / 3.;
        const double C1906 = 2. / 3.;
        const double C1907 = 5. / 3.;
        const double C1908 = 7. / 6.;
        const double C1909 = 0.9999999999999999 * C1904;
        const double C1910 = Pi * C1903;
        const double C1911 = sigmaAa + C1902;
        const double C1912 = std::pow(C1903, C1908);
        const double C1913 = 4. * C1910;
        const double C1914 = C1911 + sigmaBb;
        const double C1915 = C1909 / C1903;
        const double C1916 = C1914 + SigmaGuard;
        const double C1917 = C1915 + 1.;
        const double C1918 = 1. - C1915;
        const double C1919 = 0.22167 / C1913;
        const double C1920 = 3. / C1913;
        const double C1921 = std::sqrt(C1916);
        const double C1922 = std::pow(C1920, C1905);
        const double C1923 = std::pow(C1920, C1906);
        const double C1924 = 7.389e-6 * C1923;
        const double C1925 = 0.00081290825 * C1921;
        const double C1926 = 0.023266 * C1922;
        const double C1927 = 0.472 * C1923;
        const double C1928 = 8.723 * C1922;
        const double C1929 = std::log(C1922);
        const double C1930 = C1922 * C1929;
        const double C1931 = C1926 + C1924;
        const double C1932 = C1928 + C1927;
        const double C1933 = C1931 + 0.002568;
        const double C1934 = C1932 + C1919;
        const double C1935 = C1934 + 1.;
        const double C1936 = C1933 / C1935;
        const double C1937 = C1936 + 0.001667;
        const double C1938 = C1912 * C1937;
        const double C1939 = C1925 / C1938;
        const double C1940 = -C1939;
        const double C1941 = std::exp(C1940);

        result.vsigmaBb =
            C1903 *
            (((((0.01555 * C1929 + 0.0007 * C1930) - 0.0269) - 0.0048 * C1922) -
              (((0.0311 * C1929 + 0.0020 * C1930) - 0.048) - 0.0116 * C1922)) *
                 (0. * std::pow(C1917, C1905) + 0. * std::pow(C1918, C1905)) /
                 (3. * C1903 * 2. * (std::pow(2., C1905) - 1.)) +
             (C1941 * C1937 +
              (0. * C1941 / C1935 + C1937 * -0.00081290825 * C1941 / (2 * C1921 * C1938)) * C1914) /
                 (std::pow(C1903, 7. / 3.) *
                  std::sqrt((std::pow(C1917, C1907) + std::pow(C1918, C1907)) / 2.)));
    }

    return result;
}

void P86CorrelationSecondDerivatives(double rhoA,
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

    if (!(std::pow(3. / (4. * Pi * (rhoA + rhoB)), 1. / 3.) < 1.))
    {
        const double C46 = 2. * sigmaAb;
        const double C47 = rhoA + rhoB;
        const double C48 = rhoA - rhoB;
        const double C49 = 1. / 3.;
        const double C50 = 2. / 3.;
        const double C51 = 4. / 3.;
        const double C52 = 5. / 3.;
        const double C53 = 0.9999999999999999 * C48;
        const double C54 = Pi * C47;
        const double C55 = sigmaAa + C46;
        const double C56 = 4. * C54;
        const double C57 = C55 + sigmaBb;
        const double C58 = C53 / C47;
        const double C59 = C58 + 1.;
        const double C60 = 1. - C58;
        const double C61 = 0.22167 / C56;
        const double C62 = 3. / C56;
        const double C63 = std::pow(C62, C49);
        const double C64 = std::pow(C62, C50);
        const double C65 = 7.389e-6 * C64;
        const double C66 = 0.023266 * C63;
        const double C67 = 0.3334 * C63;
        const double C68 = 0.472 * C64;
        const double C69 = 8.723 * C63;
        const double C70 = std::sqrt(C63);
        const double C71 = 1.0529 * C70;
        const double C72 = C66 + C65;
        const double C73 = C69 + C68;
        const double C74 = C71 + C67;
        const double C75 = C72 + 0.002568;
        const double C76 = C73 + C61;
        const double C77 = C74 + 1.;
        const double C78 = C76 + 1.;
        const double C79 = 0.1423 / C77;
        const double C80 = C75 / C78;
        const double C81 = C80 + 0.001667;
        const double C86 = 1;
        const double C87 = 1;
        const double C88 = 0.9999999999999999 * C87 + 0.;
        const double C89 = Pi * C86;
        const double C90 = 4. * C89;
        const double C91 = C88 / C47 - C86 * C53 / std::pow(C47, 2);
        const double C92 = C91;
        const double C93 = -C91;
        const double C94 = -0.22167 * C90 / std::pow(C56, 2);
        const double C95 = -3. * C90 / std::pow(C56, 2);
        const double C96 = C49 * std::pow(C62, C49 - 1) * C95;
        const double C97 = C50 * std::pow(C62, C50 - 1) * C95;
        const double C98 = 0.7389e-5 * C97 + 0.;
        const double C99 = 0.023266 * C96;
        const double C100 = 0.3334 * C96;
        const double C101 = 0.472 * C97;
        const double C102 = 8.723 * C96;
        const double C103 = 1.0529 * C96 / (2 * std::sqrt(C63));
        const double C104 = C99 + C98;
        const double C105 = C102 + C101;
        const double C106 = C103 + C100;
        const double C107 = C104;
        const double C108 = C105 + C94;
        const double C109 = C106;
        const double C110 = C108;
        const double C111 = -0.1423 * C109 / std::pow(C77, 2);
        const double C112 = C107 / C78 - C110 * C75 / std::pow(C78, 2);
        const double C113 = C112;
        const double C175 = 0.;
        const double C176 =
            (C175 / C47 +
             ((-C88) / std::pow(C47, 2) - (-C86 * C53 * 2 * C47) / std::pow(C47, 4)) * C86) -
            C88 * C86 / std::pow(C47, 2);
        const double C177 = C176;
        const double C178 = -C176;
        const double C179 = (-C90 * -0.22167 * C90 * 2 * C56) / std::pow(C56, 4);
        const double C180 = (-C90 * -3. * C90 * 2 * C56) / std::pow(C56, 4);
        const double C181 = C49 * (C49 - 1) * std::pow(C62, C49 - 2) * std::pow(C95, 2) +
                            C49 * std::pow(C62, C49 - 1) * C180;
        const double C182 = C50 * (C50 - 1) * std::pow(C62, C50 - 2) * std::pow(C95, 2) +
                            C50 * std::pow(C62, C50 - 1) * C180;
        const double C183 = 0.7389e-5 * C182 + 0.;
        const double C184 = 0.023266 * C181;
        const double C185 = 0.3334 * C181;
        const double C186 = 0.472 * C182;
        const double C187 = 8.723 * C181;
        const double C188 =
            1.0529 * C181 / (2 * std::sqrt(C63)) -
            1.0529 * std::pow(C96, 2) / (std::sqrt(C63) * std::pow(2 * std::sqrt(C63), 2));
        const double C189 = C184 + C183;
        const double C190 = C187 + C186;
        const double C191 = C188 + C185;
        const double C192 = C189;
        const double C193 = C190 + C179;
        const double C194 = C191;
        const double C195 = C193;
        const double C196 =
            -0.1423 * C194 / std::pow(C77, 2) - C109 * -0.1423 * C109 * 2 * C77 / std::pow(C77, 4);
        const double C197 =
            ((C192 / C78 +
              ((-C107) / std::pow(C78, 2) - (-C110 * C75 * 2 * C78) / std::pow(C78, 4)) * C110) -
             C195 * C75 / std::pow(C78, 2)) -
            C107 * C110 / std::pow(C78, 2);
        const double C198 = C197;
        matrix.upper[0] =
            ((((((((((((((((std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                (C47 * ((6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2) *
                                             (std::pow(C47, 7. / 3.) *
                                                  std::sqrt(
                                                      (std::pow(C59, C52) + std::pow(C60, C52)) /
                                                      2.) *
                                                  C57 * C81 *
                                                  (std::exp(-0.00081290825 * std::sqrt(C57) /
                                                            (std::pow(C47, 7. / 6.) * C81)) *
                                                       0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                                       std::pow(C47, -5. / 6.) / 6. +
                                                   0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                                       std::pow(C47, 1. / 6.) *
                                                       std::exp(-0.00081290825 * std::sqrt(C57) /
                                                                (std::pow(C47, 7. / 6.) * C81)) *
                                                       0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                                       std::pow(C47, 1. / 6.) /
                                                       (6. * std::pow(std::pow(C47, 7. / 6.) * C81,
                                                                      2))) +
                                              C57 * C81 *
                                                  std::exp(-0.00081290825 * std::sqrt(C57) /
                                                           (std::pow(C47, 7. / 6.) * C81)) *
                                                  0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                                  std::pow(C47, 1. / 6.) *
                                                  std::sqrt(
                                                      (std::pow(C59, C52) + std::pow(C60, C52)) /
                                                      2.) *
                                                  7. * std::pow(C47, 4. / 3.) / 3.) -
                                         std::pow(C47, 7. / 3.) *
                                             std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                       2.) *
                                             C57 * C81 *
                                             std::exp(-0.00081290825 * std::sqrt(C57) /
                                                      (std::pow(C47, 7. / 6.) * C81)) *
                                             0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                             std::pow(C47, 1. / 6.) * 6. * std::pow(C47, 7. / 6.) *
                                             C81 * 2 * C81 * 7. * std::pow(C47, 1. / 6.) / 6.) /
                                            std::pow(6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2),
                                                     2) -
                                        (std::exp(-0.00081290825 * std::sqrt(C57) /
                                                  (std::pow(C47, 7. / 6.) * C81)) *
                                             C81 * C57 *
                                             std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                       2.) *
                                             28. * std::pow(C47, 1. / 3.) / 3. +
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                             7. * std::pow(C47, 4. / 3.) * C57 * C81 *
                                             std::exp(-0.00081290825 * std::sqrt(C57) /
                                                      (std::pow(C47, 7. / 6.) * C81)) *
                                             0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                             std::pow(C47, 1. / 6.) /
                                             (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) /
                                            3.) +
                                 (std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                      C57 * C81 *
                                      std::exp(-0.00081290825 * std::sqrt(C57) /
                                               (std::pow(C47, 7. / 6.) * C81)) *
                                      0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                      std::pow(C47, 1. / 6.) /
                                      (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                  std::exp(-0.00081290825 * std::sqrt(C57) /
                                           (std::pow(C47, 7. / 6.) * C81)) *
                                      C81 * C57 *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                      7. * std::pow(C47, 4. / 3.) / 3.)) -
                            C47 *
                                (std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     C57 * C81 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) /
                                     (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                     C81 * C57 *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     7. * std::pow(C47, 4. / 3.) / 3.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                std::pow(C47, 4. / 3.) / 3.) /
                               std::pow(
                                   std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   4) +
                           (std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * C57 *
                                C81 *
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) *
                                0.00081290825 * std::sqrt(C57) * C81 * 7. * std::pow(C47, 1. / 6.) /
                                (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                C81 * C57 *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                std::pow(C47, 4. / 3.) / 3.) /
                               std::pow(
                                   std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2)) *
                              C86 +
                          (((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) /
                               (2. * (std::pow(2., C49) - 1.)) -
                           1) *
                              C111) +
                         C96 * ((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) * 0.0843 *
                             (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                             (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                              (std::pow(2., C49) - 1.))) +
                        (((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                              std::pow(C59, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) *
                              C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                              (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                               std::pow(
                                   std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2))) -
                         C47 *
                             (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                  std::pow(
                                      std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2) *
                                  (std::exp(-0.00081290825 * std::sqrt(C57) /
                                            (std::pow(C47, 7. / 6.) * C81)) *
                                       C81 * C57 * C52 * std::pow(C59, C52 - 1) * 7. *
                                       std::pow(C47, 4. / 3.) / 3. +
                                   std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * C57 *
                                       C81 *
                                       std::exp(-0.00081290825 * std::sqrt(C57) /
                                                (std::pow(C47, 7. / 6.) * C81)) *
                                       0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                       std::pow(C47, 1. / 6.) /
                                       (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) -
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                                  C81 * C57 * std::pow(C47, 7. / 3.) * C52 *
                                  std::pow(C59, C52 - 1) * 4. *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                  std::pow(C47, 7. / 3.) *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                  std::pow(C47, 4. / 3.) / 3.) /
                             std::pow(
                                 4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     std::pow(
                                         std::pow(C47, 7. / 3.) *
                                             std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                       2.),
                                         2),
                                 2)) *
                            C92) +
                       (((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                             std::pow(C60, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                             C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                             (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                              std::pow(
                                  std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  2))) -
                        C47 *
                            (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                 std::pow(
                                     std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                 (std::exp(-0.00081290825 * std::sqrt(C57) /
                                           (std::pow(C47, 7. / 6.) * C81)) *
                                      C81 * C57 * C52 * std::pow(C60, C52 - 1) * 7. *
                                      std::pow(C47, 4. / 3.) / 3. +
                                  std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * C57 *
                                      C81 *
                                      std::exp(-0.00081290825 * std::sqrt(C57) /
                                               (std::pow(C47, 7. / 6.) * C81)) *
                                      0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                      std::pow(C47, 1. / 6.) /
                                      (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) -
                             std::exp(-0.00081290825 * std::sqrt(C57) /
                                      (std::pow(C47, 7. / 6.) * C81)) *
                                 C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) *
                                 4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                 std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                 std::pow(C47, 4. / 3.) / 3.) /
                            std::pow(
                                4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                    std::pow(
                                        std::pow(C47, 7. / 3.) *
                                            std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                      2.),
                                        2),
                                2)) *
                           C93) +
                      (std::pow(C47, 7. / 3.) *
                           std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * C113 *
                           (C47 *
                                (std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) /
                                     (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                                 (std::pow(std::pow(C47, 7. / 6.) * C81, 2) * C81 *
                                      (std::exp(-0.00081290825 * std::sqrt(C57) /
                                                (std::pow(C47, 7. / 6.) * C81)) *
                                           0.00081290825 * std::sqrt(C57) * 7. *
                                           std::pow(C47, 1. / 6.) / 6. +
                                       0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                                    (std::pow(C47, 7. / 6.) * C81)) *
                                           0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                           std::pow(C47, 1. / 6.) /
                                           (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) -
                                  C81 *
                                      std::exp(-0.00081290825 * std::sqrt(C57) /
                                               (std::pow(C47, 7. / 6.) * C81)) *
                                      0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                                      std::pow(C47, 7. / 6.) * C81 * 2 * C81 * 7. *
                                      std::pow(C47, 1. / 6.) / 6.) /
                                     std::pow(std::pow(C47, 7. / 6.) * C81, 4)) *
                                C57 +
                            (std::exp(-0.00081290825 * std::sqrt(C57) /
                                      (std::pow(C47, 7. / 6.) * C81)) +
                             C81 *
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                 std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                                C57) -
                       C113 * C47 *
                           (std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) +
                            C81 *
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) *
                                0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                           C57 * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                           std::pow(C47, 4. / 3.) / 3.) /
                          std::pow(std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2)) *
                         C86 +
                     (((((((std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                C47 *
                                (C57 * C81 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) * std::pow(C47, 7. / 3.) * C52 *
                                     std::pow(C59, C52 - 1) /
                                     (4. *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                      6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                     C81 * C57 * 7. * std::pow(C47, 4. / 3.) * C52 *
                                     std::pow(C59, C52 - 1) /
                                     (12. *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) -
                            C47 *
                                (std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     C57 * C81 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) /
                                     (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                     C81 * C57 *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     7. * std::pow(C47, 4. / 3.) / 3.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                                (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                               std::pow(
                                   std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   4) +
                           ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                                std::pow(C59, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                                (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                 std::pow(
                                     std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2)))) *
                              C86 +
                          C111 * C47 * C51 * std::pow(C59, C51 - 1) /
                              (2. * (std::pow(2., C49) - 1.))) +
                         C96 * C47 * C51 * std::pow(C59, C51 - 1) * 0.0843 *
                             (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                             (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                              (std::pow(2., C49) - 1.))) +
                        C47 *
                            ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) *
                                 C51 * (C51 - 1) * std::pow(C59, C51 - 2) /
                                 (2. * (std::pow(2., C49) - 1.)) -
                             (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                  std::pow(
                                      std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2) *
                                  std::exp(-0.00081290825 * std::sqrt(C57) /
                                           (std::pow(C47, 7. / 6.) * C81)) *
                                  C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                                  std::pow(C59, C52 - 2) -
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                                  C81 * C57 * std::pow(C47, 7. / 3.) * C52 *
                                  std::pow(C59, C52 - 1) * 4. *
                                  (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                       std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                       2 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                                   std::pow(std::pow(C47, 7. / 3.) *
                                                std::sqrt(
                                                    (std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                            2) *
                                       C52 * std::pow(C59, C52 - 1)) /
                                  (4. *
                                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                                 std::pow(
                                     4. *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                         std::pow(std::pow(C47, 7. / 3.) *
                                                      std::sqrt((std::pow(C59, C52) +
                                                                 std::pow(C60, C52)) /
                                                                2.),
                                                  2),
                                     2)) *
                            C92) +
                       C93 * C47 *
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                           C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                           (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                C52 * std::pow(C59, C52 - 1)) /
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                            std::pow(
                                4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                    std::pow(
                                        std::pow(C47, 7. / 3.) *
                                            std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                      2.),
                                        2),
                                2))) -
                      C113 * C47 *
                          (std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) +
                           C81 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) *
                               0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                               std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                          C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                          (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                           std::pow(std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2))) *
                         C92) +
                    (((((((std::pow(std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) *
                               C47 *
                               (C57 * C81 *
                                    std::exp(-0.00081290825 * std::sqrt(C57) /
                                             (std::pow(C47, 7. / 6.) * C81)) *
                                    0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                    std::pow(C47, 1. / 6.) * std::pow(C47, 7. / 3.) * C52 *
                                    std::pow(C60, C52 - 1) /
                                    (4. *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) *
                                    C81 * C57 * 7. * std::pow(C47, 4. / 3.) * C52 *
                                    std::pow(C60, C52 - 1) /
                                    (12. *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) -
                           C47 *
                               (std::pow(C47, 7. / 3.) *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                    C57 * C81 *
                                    std::exp(-0.00081290825 * std::sqrt(C57) /
                                             (std::pow(C47, 7. / 6.) * C81)) *
                                    0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                    std::pow(C47, 1. / 6.) /
                                    (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) *
                                    C81 * C57 *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                    std::pow(C47, 4. / 3.) / 3.) *
                               std::pow(C47, 7. / 3.) *
                               std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                               std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                               (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                              std::pow(
                                  std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  4) +
                          ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                               std::pow(C60, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                               C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                               (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(
                                    std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2)))) *
                             C86 +
                         C111 * C47 * C51 * std::pow(C60, C51 - 1) /
                             (2. * (std::pow(2., C49) - 1.))) +
                        C96 * C47 * C51 * std::pow(C60, C51 - 1) * 0.0843 *
                            (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                            (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                             (std::pow(2., C49) - 1.))) +
                       C92 * C47 *
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                           C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * 4. *
                           (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                C52 * std::pow(C60, C52 - 1)) /
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                            std::pow(
                                4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                    std::pow(
                                        std::pow(C47, 7. / 3.) *
                                            std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                      2.),
                                        2),
                                2))) +
                      C47 *
                          ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                               (C51 - 1) * std::pow(C60, C51 - 2) /
                               (2. * (std::pow(2., C49) - 1.)) -
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(
                                    std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) *
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) *
                                C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                                std::pow(C60, C52 - 2) -
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) *
                                4. *
                                (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                     std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                                 std::pow(
                                     std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                     C52 * std::pow(C60, C52 - 1)) /
                                (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                               std::pow(
                                   4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                       std::pow(
                                           std::pow(C47, 7. / 3.) *
                                               std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                         2.),
                                           2),
                                   2)) *
                          C93) -
                     C113 * C47 *
                         (std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) +
                          C81 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                              std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                         C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                         (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                          std::pow(std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2))) *
                        C93) +
                   ((((C47 *
                           ((6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2) *
                                 std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * C57 *
                                 (C81 *
                                      (std::exp(-0.00081290825 * std::sqrt(C57) /
                                                (std::pow(C47, 7. / 6.) * C81)) *
                                           0.00081290825 * std::sqrt(C57) * 7. *
                                           std::pow(C47, 1. / 6.) +
                                       0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                           std::pow(C47, 1. / 6.) *
                                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                                    (std::pow(C47, 7. / 6.) * C81)) *
                                           0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                           std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                                  std::exp(-0.00081290825 * std::sqrt(C57) /
                                           (std::pow(C47, 7. / 6.) * C81)) *
                                      0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                      std::pow(C47, 1. / 6.)) -
                             std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * C57 *
                                 C81 *
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                 0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                 std::pow(C47, 1. / 6.) * 12. * C81 * std::pow(C47, 14. / 6.)) /
                                std::pow(6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2), 2) -
                            (std::exp(-0.00081290825 * std::sqrt(C57) /
                                      (std::pow(C47, 7. / 6.) * C81)) +
                             C81 *
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                 std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                                C57 * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                7. * std::pow(C47, 4. / 3.) / 3.) /
                           std::pow(std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) +
                       (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                        C81 *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                            0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                            std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                           C57 /
                           (std::pow(C47, 7. / 3.) *
                            std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                          C86 -
                      C92 * C47 *
                          (std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) +
                           C81 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) *
                               0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                               std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                          C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                          (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                           std::pow(std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2))) -
                     C93 * C47 *
                         (std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) +
                          C81 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                              std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                         C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                         (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                          std::pow(std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2))) +
                    C113 * C47 *
                        (std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                             0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                             std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                         (std::pow(std::pow(C47, 7. / 6.) * C81, 2) *
                              (C81 * 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                                   std::exp(-0.00081290825 * std::sqrt(C57) /
                                            (std::pow(C47, 7. / 6.) * C81)) *
                                   0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                   std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) *
                                   0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) -
                          C81 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * 2 * C81 *
                              std::pow(C47, 14. / 6.)) /
                             std::pow(std::pow(C47, 7. / 6.) * C81, 4)) *
                        C57 /
                        (std::pow(C47, 7. / 3.) *
                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                       C113) +
                  (((((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) /
                         (2. * (std::pow(2., C49) - 1.)) -
                     1) *
                        C86 +
                    C92 * C47 * C51 * std::pow(C59, C51 - 1) / (2. * (std::pow(2., C49) - 1.))) +
                   C93 * C47 * C51 * std::pow(C60, C51 - 1) / (2. * (std::pow(2., C49) - 1.))) *
                      C111) +
                 (((C86 * ((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) * 0.0843 *
                        (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                        (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                         (std::pow(2., C49) - 1.)) +
                    (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                         (std::pow(2., C49) - 1.) * C96 * C47 * -0.11785983 *
                         ((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) /
                         (std::sqrt(C63) * std::pow(2 * std::sqrt(C63), 2)) -
                     C96 * C47 * ((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) * 0.0843 *
                         (1.3981 / (2 * std::sqrt(C63)) + 0.2611) * 2 *
                         (1.3981 / (2 * std::sqrt(C63)) + 0.2611) *
                         ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.) * 2. *
                         (std::pow(2., C49) - 1.)) /
                        std::pow(std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                                     (std::pow(2., C49) - 1.),
                                 2)) +
                   C92 * C47 * C51 * std::pow(C59, C51 - 1) * 0.0843 *
                       (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                       (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                        (std::pow(2., C49) - 1.))) +
                  C93 * C47 * C51 * std::pow(C60, C51 - 1) * 0.0843 *
                      (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                      (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                       (std::pow(2., C49) - 1.))) *
                     C96) +
                C47 *
                    (((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) /
                         (2. * (std::pow(2., C49) - 1.)) -
                     1) *
                    C196) +
               C181 * C47 * ((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) * 0.0843 *
                   (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                   (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                    (std::pow(2., C49) - 1.))) +
              C47 *
                  ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                       std::pow(C59, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                   std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                       C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                       (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                        std::pow(std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                 2))) *
                  C177) +
             C47 *
                 ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                      std::pow(C60, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                  std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) * C81 *
                      C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                       std::pow(std::pow(C47, 7. / 3.) *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                2))) *
                 C178) +
            C198 * C47 *
                (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                 C81 * std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                     0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                     std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                C57 /
                (std::pow(C47, 7. / 3.) *
                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.));

        const double C114 = 1;
        const double C115 = -1;
        const double C116 = 0.9999999999999999 * C115 + 0.;
        const double C117 = Pi * C114;
        const double C118 = 4. * C117;
        const double C119 = C116 / C47 - C114 * C53 / std::pow(C47, 2);
        const double C120 = C119;
        const double C121 = -C119;
        const double C122 = -0.22167 * C118 / std::pow(C56, 2);
        const double C123 = -3. * C118 / std::pow(C56, 2);
        const double C124 = C49 * std::pow(C62, C49 - 1) * C123;
        const double C125 = C50 * std::pow(C62, C50 - 1) * C123;
        const double C126 = 0.7389e-5 * C125 + 0.;
        const double C127 = 0.023266 * C124;
        const double C128 = 0.3334 * C124;
        const double C129 = 0.472 * C125;
        const double C130 = 8.723 * C124;
        const double C131 = 1.0529 * C124 / (2 * std::sqrt(C63));
        const double C132 = C127 + C126;
        const double C133 = C130 + C129;
        const double C134 = C131 + C128;
        const double C135 = C132;
        const double C136 = C133 + C122;
        const double C137 = C134;
        const double C138 = C136;
        const double C139 = -0.1423 * C137 / std::pow(C77, 2);
        const double C140 = C135 / C78 - C138 * C75 / std::pow(C78, 2);
        const double C141 = C140;
        const double C199 = 0.;
        const double C200 =
            (C199 / C47 +
             ((-C88) / std::pow(C47, 2) - (-C86 * C53 * 2 * C47) / std::pow(C47, 4)) * C114) -
            C116 * C86 / std::pow(C47, 2);
        const double C201 = C200;
        const double C202 = -C200;
        const double C203 = (-C118 * -0.22167 * C90 * 2 * C56) / std::pow(C56, 4);
        const double C204 = (-C118 * -3. * C90 * 2 * C56) / std::pow(C56, 4);
        const double C205 = C49 * (C49 - 1) * std::pow(C62, C49 - 2) * C95 * C123 +
                            C49 * std::pow(C62, C49 - 1) * C204;
        const double C206 = C50 * (C50 - 1) * std::pow(C62, C50 - 2) * C95 * C123 +
                            C50 * std::pow(C62, C50 - 1) * C204;
        const double C207 = 0.7389e-5 * C206 + 0.;
        const double C208 = 0.023266 * C205;
        const double C209 = 0.3334 * C205;
        const double C210 = 0.472 * C206;
        const double C211 = 8.723 * C205;
        const double C212 =
            1.0529 * C205 / (2 * std::sqrt(C63)) -
            C124 * 1.0529 * C96 / (std::sqrt(C63) * std::pow(2 * std::sqrt(C63), 2));
        const double C213 = C208 + C207;
        const double C214 = C211 + C210;
        const double C215 = C212 + C209;
        const double C216 = C213;
        const double C217 = C214 + C203;
        const double C218 = C215;
        const double C219 = C217;
        const double C220 =
            -0.1423 * C218 / std::pow(C77, 2) - C137 * -0.1423 * C109 * 2 * C77 / std::pow(C77, 4);
        const double C221 =
            ((C216 / C78 +
              ((-C107) / std::pow(C78, 2) - (-C110 * C75 * 2 * C78) / std::pow(C78, 4)) * C138) -
             C219 * C75 / std::pow(C78, 2)) -
            C135 * C110 / std::pow(C78, 2);
        const double C222 = C221;
        matrix.upper[1] =
            ((((((((((((((((std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                (C47 * ((6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2) *
                                             (std::pow(C47, 7. / 3.) *
                                                  std::sqrt(
                                                      (std::pow(C59, C52) + std::pow(C60, C52)) /
                                                      2.) *
                                                  C57 * C81 *
                                                  (std::exp(-0.00081290825 * std::sqrt(C57) /
                                                            (std::pow(C47, 7. / 6.) * C81)) *
                                                       0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                                       std::pow(C47, -5. / 6.) / 6. +
                                                   0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                                       std::pow(C47, 1. / 6.) *
                                                       std::exp(-0.00081290825 * std::sqrt(C57) /
                                                                (std::pow(C47, 7. / 6.) * C81)) *
                                                       0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                                       std::pow(C47, 1. / 6.) /
                                                       (6. * std::pow(std::pow(C47, 7. / 6.) * C81,
                                                                      2))) +
                                              C57 * C81 *
                                                  std::exp(-0.00081290825 * std::sqrt(C57) /
                                                           (std::pow(C47, 7. / 6.) * C81)) *
                                                  0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                                  std::pow(C47, 1. / 6.) *
                                                  std::sqrt(
                                                      (std::pow(C59, C52) + std::pow(C60, C52)) /
                                                      2.) *
                                                  7. * std::pow(C47, 4. / 3.) / 3.) -
                                         std::pow(C47, 7. / 3.) *
                                             std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                       2.) *
                                             C57 * C81 *
                                             std::exp(-0.00081290825 * std::sqrt(C57) /
                                                      (std::pow(C47, 7. / 6.) * C81)) *
                                             0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                             std::pow(C47, 1. / 6.) * 6. * std::pow(C47, 7. / 6.) *
                                             C81 * 2 * C81 * 7. * std::pow(C47, 1. / 6.) / 6.) /
                                            std::pow(6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2),
                                                     2) -
                                        (std::exp(-0.00081290825 * std::sqrt(C57) /
                                                  (std::pow(C47, 7. / 6.) * C81)) *
                                             C81 * C57 *
                                             std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                       2.) *
                                             28. * std::pow(C47, 1. / 3.) / 3. +
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                             7. * std::pow(C47, 4. / 3.) * C57 * C81 *
                                             std::exp(-0.00081290825 * std::sqrt(C57) /
                                                      (std::pow(C47, 7. / 6.) * C81)) *
                                             0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                             std::pow(C47, 1. / 6.) /
                                             (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) /
                                            3.) +
                                 (std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                      C57 * C81 *
                                      std::exp(-0.00081290825 * std::sqrt(C57) /
                                               (std::pow(C47, 7. / 6.) * C81)) *
                                      0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                      std::pow(C47, 1. / 6.) /
                                      (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                  std::exp(-0.00081290825 * std::sqrt(C57) /
                                           (std::pow(C47, 7. / 6.) * C81)) *
                                      C81 * C57 *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                      7. * std::pow(C47, 4. / 3.) / 3.)) -
                            C47 *
                                (std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     C57 * C81 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) /
                                     (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                     C81 * C57 *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     7. * std::pow(C47, 4. / 3.) / 3.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                std::pow(C47, 4. / 3.) / 3.) /
                               std::pow(
                                   std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   4) +
                           (std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * C57 *
                                C81 *
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) *
                                0.00081290825 * std::sqrt(C57) * C81 * 7. * std::pow(C47, 1. / 6.) /
                                (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                C81 * C57 *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                std::pow(C47, 4. / 3.) / 3.) /
                               std::pow(
                                   std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2)) *
                              C86 +
                          (((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) /
                               (2. * (std::pow(2., C49) - 1.)) -
                           1) *
                              C111) +
                         C96 * ((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) * 0.0843 *
                             (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                             (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                              (std::pow(2., C49) - 1.))) +
                        (((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                              std::pow(C59, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) *
                              C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                              (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                               std::pow(
                                   std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2))) -
                         C47 *
                             (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                  std::pow(
                                      std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2) *
                                  (std::exp(-0.00081290825 * std::sqrt(C57) /
                                            (std::pow(C47, 7. / 6.) * C81)) *
                                       C81 * C57 * C52 * std::pow(C59, C52 - 1) * 7. *
                                       std::pow(C47, 4. / 3.) / 3. +
                                   std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * C57 *
                                       C81 *
                                       std::exp(-0.00081290825 * std::sqrt(C57) /
                                                (std::pow(C47, 7. / 6.) * C81)) *
                                       0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                       std::pow(C47, 1. / 6.) /
                                       (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) -
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                                  C81 * C57 * std::pow(C47, 7. / 3.) * C52 *
                                  std::pow(C59, C52 - 1) * 4. *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                  std::pow(C47, 7. / 3.) *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                  std::pow(C47, 4. / 3.) / 3.) /
                             std::pow(
                                 4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     std::pow(
                                         std::pow(C47, 7. / 3.) *
                                             std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                       2.),
                                         2),
                                 2)) *
                            C92) +
                       (((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                             std::pow(C60, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                             C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                             (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                              std::pow(
                                  std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  2))) -
                        C47 *
                            (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                 std::pow(
                                     std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                 (std::exp(-0.00081290825 * std::sqrt(C57) /
                                           (std::pow(C47, 7. / 6.) * C81)) *
                                      C81 * C57 * C52 * std::pow(C60, C52 - 1) * 7. *
                                      std::pow(C47, 4. / 3.) / 3. +
                                  std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * C57 *
                                      C81 *
                                      std::exp(-0.00081290825 * std::sqrt(C57) /
                                               (std::pow(C47, 7. / 6.) * C81)) *
                                      0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                      std::pow(C47, 1. / 6.) /
                                      (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) -
                             std::exp(-0.00081290825 * std::sqrt(C57) /
                                      (std::pow(C47, 7. / 6.) * C81)) *
                                 C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) *
                                 4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                 std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                 std::pow(C47, 4. / 3.) / 3.) /
                            std::pow(
                                4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                    std::pow(
                                        std::pow(C47, 7. / 3.) *
                                            std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                      2.),
                                        2),
                                2)) *
                           C93) +
                      (std::pow(C47, 7. / 3.) *
                           std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * C113 *
                           (C47 *
                                (std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) /
                                     (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                                 (std::pow(std::pow(C47, 7. / 6.) * C81, 2) * C81 *
                                      (std::exp(-0.00081290825 * std::sqrt(C57) /
                                                (std::pow(C47, 7. / 6.) * C81)) *
                                           0.00081290825 * std::sqrt(C57) * 7. *
                                           std::pow(C47, 1. / 6.) / 6. +
                                       0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                                    (std::pow(C47, 7. / 6.) * C81)) *
                                           0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                           std::pow(C47, 1. / 6.) /
                                           (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) -
                                  C81 *
                                      std::exp(-0.00081290825 * std::sqrt(C57) /
                                               (std::pow(C47, 7. / 6.) * C81)) *
                                      0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                                      std::pow(C47, 7. / 6.) * C81 * 2 * C81 * 7. *
                                      std::pow(C47, 1. / 6.) / 6.) /
                                     std::pow(std::pow(C47, 7. / 6.) * C81, 4)) *
                                C57 +
                            (std::exp(-0.00081290825 * std::sqrt(C57) /
                                      (std::pow(C47, 7. / 6.) * C81)) +
                             C81 *
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                 std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                                C57) -
                       C113 * C47 *
                           (std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) +
                            C81 *
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) *
                                0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                           C57 * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                           std::pow(C47, 4. / 3.) / 3.) /
                          std::pow(std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2)) *
                         C114 +
                     (((((((std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                C47 *
                                (C57 * C81 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) * std::pow(C47, 7. / 3.) * C52 *
                                     std::pow(C59, C52 - 1) /
                                     (4. *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                      6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                     C81 * C57 * 7. * std::pow(C47, 4. / 3.) * C52 *
                                     std::pow(C59, C52 - 1) /
                                     (12. *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) -
                            C47 *
                                (std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     C57 * C81 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) /
                                     (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                     C81 * C57 *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     7. * std::pow(C47, 4. / 3.) / 3.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                                (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                               std::pow(
                                   std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   4) +
                           ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                                std::pow(C59, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                                (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                 std::pow(
                                     std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2)))) *
                              C86 +
                          C111 * C47 * C51 * std::pow(C59, C51 - 1) /
                              (2. * (std::pow(2., C49) - 1.))) +
                         C96 * C47 * C51 * std::pow(C59, C51 - 1) * 0.0843 *
                             (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                             (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                              (std::pow(2., C49) - 1.))) +
                        C47 *
                            ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) *
                                 C51 * (C51 - 1) * std::pow(C59, C51 - 2) /
                                 (2. * (std::pow(2., C49) - 1.)) -
                             (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                  std::pow(
                                      std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2) *
                                  std::exp(-0.00081290825 * std::sqrt(C57) /
                                           (std::pow(C47, 7. / 6.) * C81)) *
                                  C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                                  std::pow(C59, C52 - 2) -
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                                  C81 * C57 * std::pow(C47, 7. / 3.) * C52 *
                                  std::pow(C59, C52 - 1) * 4. *
                                  (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                       std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                       2 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                                   std::pow(std::pow(C47, 7. / 3.) *
                                                std::sqrt(
                                                    (std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                            2) *
                                       C52 * std::pow(C59, C52 - 1)) /
                                  (4. *
                                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                                 std::pow(
                                     4. *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                         std::pow(std::pow(C47, 7. / 3.) *
                                                      std::sqrt((std::pow(C59, C52) +
                                                                 std::pow(C60, C52)) /
                                                                2.),
                                                  2),
                                     2)) *
                            C92) +
                       C93 * C47 *
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                           C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                           (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                C52 * std::pow(C59, C52 - 1)) /
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                            std::pow(
                                4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                    std::pow(
                                        std::pow(C47, 7. / 3.) *
                                            std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                      2.),
                                        2),
                                2))) -
                      C113 * C47 *
                          (std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) +
                           C81 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) *
                               0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                               std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                          C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                          (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                           std::pow(std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2))) *
                         C120) +
                    (((((((std::pow(std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) *
                               C47 *
                               (C57 * C81 *
                                    std::exp(-0.00081290825 * std::sqrt(C57) /
                                             (std::pow(C47, 7. / 6.) * C81)) *
                                    0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                    std::pow(C47, 1. / 6.) * std::pow(C47, 7. / 3.) * C52 *
                                    std::pow(C60, C52 - 1) /
                                    (4. *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) *
                                    C81 * C57 * 7. * std::pow(C47, 4. / 3.) * C52 *
                                    std::pow(C60, C52 - 1) /
                                    (12. *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) -
                           C47 *
                               (std::pow(C47, 7. / 3.) *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                    C57 * C81 *
                                    std::exp(-0.00081290825 * std::sqrt(C57) /
                                             (std::pow(C47, 7. / 6.) * C81)) *
                                    0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                    std::pow(C47, 1. / 6.) /
                                    (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) *
                                    C81 * C57 *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                    std::pow(C47, 4. / 3.) / 3.) *
                               std::pow(C47, 7. / 3.) *
                               std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                               std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                               (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                              std::pow(
                                  std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  4) +
                          ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                               std::pow(C60, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                               C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                               (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(
                                    std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2)))) *
                             C86 +
                         C111 * C47 * C51 * std::pow(C60, C51 - 1) /
                             (2. * (std::pow(2., C49) - 1.))) +
                        C96 * C47 * C51 * std::pow(C60, C51 - 1) * 0.0843 *
                            (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                            (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                             (std::pow(2., C49) - 1.))) +
                       C92 * C47 *
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                           C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * 4. *
                           (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                C52 * std::pow(C60, C52 - 1)) /
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                            std::pow(
                                4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                    std::pow(
                                        std::pow(C47, 7. / 3.) *
                                            std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                      2.),
                                        2),
                                2))) +
                      C47 *
                          ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                               (C51 - 1) * std::pow(C60, C51 - 2) /
                               (2. * (std::pow(2., C49) - 1.)) -
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(
                                    std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) *
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) *
                                C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                                std::pow(C60, C52 - 2) -
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) *
                                4. *
                                (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                     std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                                 std::pow(
                                     std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                     C52 * std::pow(C60, C52 - 1)) /
                                (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                               std::pow(
                                   4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                       std::pow(
                                           std::pow(C47, 7. / 3.) *
                                               std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                         2.),
                                           2),
                                   2)) *
                          C93) -
                     C113 * C47 *
                         (std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) +
                          C81 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                              std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                         C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                         (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                          std::pow(std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2))) *
                        C121) +
                   ((((C47 *
                           ((6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2) *
                                 std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * C57 *
                                 (C81 *
                                      (std::exp(-0.00081290825 * std::sqrt(C57) /
                                                (std::pow(C47, 7. / 6.) * C81)) *
                                           0.00081290825 * std::sqrt(C57) * 7. *
                                           std::pow(C47, 1. / 6.) +
                                       0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                           std::pow(C47, 1. / 6.) *
                                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                                    (std::pow(C47, 7. / 6.) * C81)) *
                                           0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                           std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                                  std::exp(-0.00081290825 * std::sqrt(C57) /
                                           (std::pow(C47, 7. / 6.) * C81)) *
                                      0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                      std::pow(C47, 1. / 6.)) -
                             std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * C57 *
                                 C81 *
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                 0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                 std::pow(C47, 1. / 6.) * 12. * C81 * std::pow(C47, 14. / 6.)) /
                                std::pow(6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2), 2) -
                            (std::exp(-0.00081290825 * std::sqrt(C57) /
                                      (std::pow(C47, 7. / 6.) * C81)) +
                             C81 *
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                 std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                                C57 * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                7. * std::pow(C47, 4. / 3.) / 3.) /
                           std::pow(std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) +
                       (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                        C81 *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                            0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                            std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                           C57 /
                           (std::pow(C47, 7. / 3.) *
                            std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                          C86 -
                      C92 * C47 *
                          (std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) +
                           C81 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) *
                               0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                               std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                          C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                          (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                           std::pow(std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2))) -
                     C93 * C47 *
                         (std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) +
                          C81 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                              std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                         C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                         (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                          std::pow(std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2))) +
                    C113 * C47 *
                        (std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                             0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                             std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                         (std::pow(std::pow(C47, 7. / 6.) * C81, 2) *
                              (C81 * 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                                   std::exp(-0.00081290825 * std::sqrt(C57) /
                                            (std::pow(C47, 7. / 6.) * C81)) *
                                   0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                   std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) *
                                   0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) -
                          C81 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * 2 * C81 *
                              std::pow(C47, 14. / 6.)) /
                             std::pow(std::pow(C47, 7. / 6.) * C81, 4)) *
                        C57 /
                        (std::pow(C47, 7. / 3.) *
                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                       C141) +
                  (((((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) /
                         (2. * (std::pow(2., C49) - 1.)) -
                     1) *
                        C86 +
                    C92 * C47 * C51 * std::pow(C59, C51 - 1) / (2. * (std::pow(2., C49) - 1.))) +
                   C93 * C47 * C51 * std::pow(C60, C51 - 1) / (2. * (std::pow(2., C49) - 1.))) *
                      C139) +
                 (((C86 * ((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) * 0.0843 *
                        (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                        (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                         (std::pow(2., C49) - 1.)) +
                    (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                         (std::pow(2., C49) - 1.) * C96 * C47 * -0.11785983 *
                         ((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) /
                         (std::sqrt(C63) * std::pow(2 * std::sqrt(C63), 2)) -
                     C96 * C47 * ((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) * 0.0843 *
                         (1.3981 / (2 * std::sqrt(C63)) + 0.2611) * 2 *
                         (1.3981 / (2 * std::sqrt(C63)) + 0.2611) *
                         ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.) * 2. *
                         (std::pow(2., C49) - 1.)) /
                        std::pow(std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                                     (std::pow(2., C49) - 1.),
                                 2)) +
                   C92 * C47 * C51 * std::pow(C59, C51 - 1) * 0.0843 *
                       (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                       (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                        (std::pow(2., C49) - 1.))) +
                  C93 * C47 * C51 * std::pow(C60, C51 - 1) * 0.0843 *
                      (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                      (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                       (std::pow(2., C49) - 1.))) *
                     C124) +
                C47 *
                    (((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) /
                         (2. * (std::pow(2., C49) - 1.)) -
                     1) *
                    C220) +
               C205 * C47 * ((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) * 0.0843 *
                   (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                   (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                    (std::pow(2., C49) - 1.))) +
              C47 *
                  ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                       std::pow(C59, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                   std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                       C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                       (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                        std::pow(std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                 2))) *
                  C201) +
             C47 *
                 ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                      std::pow(C60, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                  std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) * C81 *
                      C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                       std::pow(std::pow(C47, 7. / 3.) *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                2))) *
                 C202) +
            C222 * C47 *
                (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                 C81 * std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                     0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                     std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                C57 /
                (std::pow(C47, 7. / 3.) *
                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.));

        const double C223 = -24. * Pi;
        const double C224 = 0.88668 * Pi;
        const double C225 = 2. * sigmaAb;
        const double C226 = 12. * Pi;
        const double C227 = rhoA + rhoB;
        const double C228 = rhoA - rhoB;
        const double C229 = -2. / 3.;
        const double C230 = -1. / 3.;
        const double C231 = 1. / 3.;
        const double C232 = 2. / 3.;
        const double C233 = 4. / 3.;
        const double C234 = 5. / 3.;
        const double C235 = 7. / 6.;
        const double C236 = 0.9999999999999999 * C227;
        const double C237 = 0.9999999999999999 * C228;
        const double C238 = Pi * C227;
        const double C239 = sigmaAa + C225;
        const double C240 = std::pow(C227, C233);
        const double C241 = std::pow(C227, C235);
        const double C242 = 4. * C238;
        const double C243 = C239 + sigmaBb;
        const double C244 = C236 - C237;
        const double C245 = C237 / C227;
        const double C246 = C245 + 1.;
        const double C247 = 1. - C245;
        const double C248 = 0.22167 / C242;
        const double C249 = 3. / C242;
        const double C250 = std::sqrt(C243);
        const double C251 = std::pow(C242, 2);
        const double C252 = 0.00081290825 * C250;
        const double C253 = 3. * C251;
        const double C254 = C224 / C251;
        const double C255 = std::pow(C246, C234);
        const double C256 = std::pow(C247, C234);
        const double C257 = std::pow(C249, C229);
        const double C258 = std::pow(C249, C230);
        const double C259 = std::pow(C249, C231);
        const double C260 = std::pow(C249, C232);
        const double C261 = 7.389e-6 * C260;
        const double C262 = 0.023266 * C259;
        const double C263 = 0.472 * C260;
        const double C264 = 8.723 * C259;
        const double C265 = C257 * C226;
        const double C266 = C258 * C223;
        const double C267 = C255 + C256;
        const double C268 = -8.723 * C265;
        const double C269 = -0.023266 * C265;
        const double C270 = 7.389e-6 * C266;
        const double C271 = 0.472 * C266;
        const double C272 = C262 + C261;
        const double C273 = C264 + C263;
        const double C274 = C267 / 2.;
        const double C275 = C268 + C271;
        const double C276 = C269 + C270;
        const double C277 = C272 + 0.002568;
        const double C278 = C273 + C248;
        const double C279 = std::sqrt(C274);
        const double C280 = C240 * C279;
        const double C281 = C278 + 1.;
        const double C282 = C275 / C253;
        const double C283 = C281 * C276;
        const double C284 = C282 - C254;
        const double C285 = C277 / C281;
        const double C286 = std::pow(C281, 2);
        const double C287 = C277 * C284;
        const double C288 = C285 + 0.001667;
        const double C289 = C283 / C253;
        const double C290 = C288 * C241;
        const double C291 = C289 - C287;
        const double C292 = C252 / C290;
        const double C293 = -C292;
        const double C294 = std::exp(C293);
        matrix.upper[2] =
            C294 * (1. - C252 / (2. * C290)) *
                (C280 * C291 / C286 - C288 * (C240 *
                                                  (std::pow(C246, C232) * 5. * C244 +
                                                   std::pow(C247, C232) * -5. * C244) /
                                                  (6. * std::pow(C227, 2) * 2 * C279) +
                                              C279 * 4. * std::pow(C227, C231) / 3.)) /
                std::pow(C280, 2) +
            C288 * 0.00081290825 * (C288 * 7. * std::pow(C227, 1. / 6.) / 6. + C241 * C291 / C286) *
                (3. - C292) * C294 * C250 / (2. * std::pow(C290, 2) * C280);

        const double C295 = -24. * Pi;
        const double C296 = 0.88668 * Pi;
        const double C297 = 2. * sigmaAb;
        const double C298 = 12. * Pi;
        const double C299 = rhoA + rhoB;
        const double C300 = rhoA - rhoB;
        const double C301 = -2. / 3.;
        const double C302 = -1. / 3.;
        const double C303 = 1. / 3.;
        const double C304 = 2. / 3.;
        const double C305 = 4. / 3.;
        const double C306 = 5. / 3.;
        const double C307 = 7. / 6.;
        const double C308 = 0.9999999999999999 * C299;
        const double C309 = 0.9999999999999999 * C300;
        const double C310 = Pi * C299;
        const double C311 = sigmaAa + C297;
        const double C312 = std::pow(C299, C305);
        const double C313 = std::pow(C299, C307);
        const double C314 = 4. * C310;
        const double C315 = C311 + sigmaBb;
        const double C316 = C308 - C309;
        const double C317 = C309 / C299;
        const double C318 = C317 + 1.;
        const double C319 = 1. - C317;
        const double C320 = 0.22167 / C314;
        const double C321 = 3. / C314;
        const double C322 = std::sqrt(C315);
        const double C323 = std::pow(C314, 2);
        const double C324 = 0.00081290825 * C322;
        const double C325 = 3. * C323;
        const double C326 = C296 / C323;
        const double C327 = std::pow(C318, C306);
        const double C328 = std::pow(C319, C306);
        const double C329 = std::pow(C321, C301);
        const double C330 = std::pow(C321, C302);
        const double C331 = std::pow(C321, C303);
        const double C332 = std::pow(C321, C304);
        const double C333 = 7.389e-6 * C332;
        const double C334 = 0.023266 * C331;
        const double C335 = 0.472 * C332;
        const double C336 = 8.723 * C331;
        const double C337 = C329 * C298;
        const double C338 = C330 * C295;
        const double C339 = C327 + C328;
        const double C340 = -8.723 * C337;
        const double C341 = -0.023266 * C337;
        const double C342 = 7.389e-6 * C338;
        const double C343 = 0.472 * C338;
        const double C344 = C334 + C333;
        const double C345 = C336 + C335;
        const double C346 = C339 / 2.;
        const double C347 = C340 + C343;
        const double C348 = C341 + C342;
        const double C349 = C344 + 0.002568;
        const double C350 = C345 + C320;
        const double C351 = std::sqrt(C346);
        const double C352 = C312 * C351;
        const double C353 = C350 + 1.;
        const double C354 = C347 / C325;
        const double C355 = C353 * C348;
        const double C356 = C354 - C326;
        const double C357 = C349 / C353;
        const double C358 = std::pow(C353, 2);
        const double C359 = C349 * C356;
        const double C360 = C357 + 0.001667;
        const double C361 = C355 / C325;
        const double C362 = C360 * C313;
        const double C363 = C361 - C359;
        const double C364 = C324 / C362;
        const double C365 = -C364;
        const double C366 = std::exp(C365);
        matrix.upper[3] =
            2. * (C366 * (1. - C324 / (2. * C362)) *
                      (C352 * C363 / C358 - C360 * (C312 *
                                                        (std::pow(C318, C304) * 5. * C316 +
                                                         std::pow(C319, C304) * -5. * C316) /
                                                        (6. * std::pow(C299, 2) * 2 * C351) +
                                                    C351 * 4. * std::pow(C299, C303) / 3.)) /
                      std::pow(C352, 2) +
                  C360 * 0.00081290825 *
                      (C360 * 7. * std::pow(C299, 1. / 6.) / 6. + C313 * C363 / C358) *
                      (3. - C364) * C366 * C322 / (2. * std::pow(C362, 2) * C352));

        const double C367 = -24. * Pi;
        const double C368 = 0.88668 * Pi;
        const double C369 = 2. * sigmaAb;
        const double C370 = 12. * Pi;
        const double C371 = rhoA + rhoB;
        const double C372 = rhoA - rhoB;
        const double C373 = -2. / 3.;
        const double C374 = -1. / 3.;
        const double C375 = 1. / 3.;
        const double C376 = 2. / 3.;
        const double C377 = 4. / 3.;
        const double C378 = 5. / 3.;
        const double C379 = 7. / 6.;
        const double C380 = 0.9999999999999999 * C371;
        const double C381 = 0.9999999999999999 * C372;
        const double C382 = Pi * C371;
        const double C383 = sigmaAa + C369;
        const double C384 = std::pow(C371, C377);
        const double C385 = std::pow(C371, C379);
        const double C386 = 4. * C382;
        const double C387 = C383 + sigmaBb;
        const double C388 = C380 - C381;
        const double C389 = C381 / C371;
        const double C390 = C389 + 1.;
        const double C391 = 1. - C389;
        const double C392 = 0.22167 / C386;
        const double C393 = 3. / C386;
        const double C394 = std::sqrt(C387);
        const double C395 = std::pow(C386, 2);
        const double C396 = 0.00081290825 * C394;
        const double C397 = 3. * C395;
        const double C398 = C368 / C395;
        const double C399 = std::pow(C390, C378);
        const double C400 = std::pow(C391, C378);
        const double C401 = std::pow(C393, C373);
        const double C402 = std::pow(C393, C374);
        const double C403 = std::pow(C393, C375);
        const double C404 = std::pow(C393, C376);
        const double C405 = 7.389e-6 * C404;
        const double C406 = 0.023266 * C403;
        const double C407 = 0.472 * C404;
        const double C408 = 8.723 * C403;
        const double C409 = C401 * C370;
        const double C410 = C402 * C367;
        const double C411 = C399 + C400;
        const double C412 = -8.723 * C409;
        const double C413 = -0.023266 * C409;
        const double C414 = 7.389e-6 * C410;
        const double C415 = 0.472 * C410;
        const double C416 = C406 + C405;
        const double C417 = C408 + C407;
        const double C418 = C411 / 2.;
        const double C419 = C412 + C415;
        const double C420 = C413 + C414;
        const double C421 = C416 + 0.002568;
        const double C422 = C417 + C392;
        const double C423 = std::sqrt(C418);
        const double C424 = C384 * C423;
        const double C425 = C422 + 1.;
        const double C426 = C419 / C397;
        const double C427 = C425 * C420;
        const double C428 = C426 - C398;
        const double C429 = C421 / C425;
        const double C430 = std::pow(C425, 2);
        const double C431 = C421 * C428;
        const double C432 = C429 + 0.001667;
        const double C433 = C427 / C397;
        const double C434 = C432 * C385;
        const double C435 = C433 - C431;
        const double C436 = C396 / C434;
        const double C437 = -C436;
        const double C438 = std::exp(C437);
        matrix.upper[4] =
            C438 * (1. - C396 / (2. * C434)) *
                (C424 * C435 / C430 - C432 * (C384 *
                                                  (std::pow(C390, C376) * 5. * C388 +
                                                   std::pow(C391, C376) * -5. * C388) /
                                                  (6. * std::pow(C371, 2) * 2 * C423) +
                                              C423 * 4. * std::pow(C371, C375) / 3.)) /
                std::pow(C424, 2) +
            C432 * 0.00081290825 * (C432 * 7. * std::pow(C371, 1. / 6.) / 6. + C385 * C435 / C430) *
                (3. - C436) * C438 * C394 / (2. * std::pow(C434, 2) * C424);

        const double C439 = 0.;
        const double C440 =
            (C439 / C47 +
             ((-C116) / std::pow(C47, 2) - (-C114 * C53 * 2 * C47) / std::pow(C47, 4)) * C114) -
            C116 * C114 / std::pow(C47, 2);
        const double C441 = C440;
        const double C442 = -C440;
        const double C443 = (-C118 * -0.22167 * C118 * 2 * C56) / std::pow(C56, 4);
        const double C444 = (-C118 * -3. * C118 * 2 * C56) / std::pow(C56, 4);
        const double C445 = C49 * (C49 - 1) * std::pow(C62, C49 - 2) * std::pow(C123, 2) +
                            C49 * std::pow(C62, C49 - 1) * C444;
        const double C446 = C50 * (C50 - 1) * std::pow(C62, C50 - 2) * std::pow(C123, 2) +
                            C50 * std::pow(C62, C50 - 1) * C444;
        const double C447 = 0.7389e-5 * C446 + 0.;
        const double C448 = 0.023266 * C445;
        const double C449 = 0.3334 * C445;
        const double C450 = 0.472 * C446;
        const double C451 = 8.723 * C445;
        const double C452 =
            1.0529 * C445 / (2 * std::sqrt(C63)) -
            1.0529 * std::pow(C124, 2) / (std::sqrt(C63) * std::pow(2 * std::sqrt(C63), 2));
        const double C453 = C448 + C447;
        const double C454 = C451 + C450;
        const double C455 = C452 + C449;
        const double C456 = C453;
        const double C457 = C454 + C443;
        const double C458 = C455;
        const double C459 = C457;
        const double C460 =
            -0.1423 * C458 / std::pow(C77, 2) - C137 * -0.1423 * C137 * 2 * C77 / std::pow(C77, 4);
        const double C461 =
            ((C456 / C78 +
              ((-C135) / std::pow(C78, 2) - (-C138 * C75 * 2 * C78) / std::pow(C78, 4)) * C138) -
             C459 * C75 / std::pow(C78, 2)) -
            C135 * C138 / std::pow(C78, 2);
        const double C462 = C461;
        matrix.upper[5] =
            ((((((((((((((((std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                (C47 * ((6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2) *
                                             (std::pow(C47, 7. / 3.) *
                                                  std::sqrt(
                                                      (std::pow(C59, C52) + std::pow(C60, C52)) /
                                                      2.) *
                                                  C57 * C81 *
                                                  (std::exp(-0.00081290825 * std::sqrt(C57) /
                                                            (std::pow(C47, 7. / 6.) * C81)) *
                                                       0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                                       std::pow(C47, -5. / 6.) / 6. +
                                                   0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                                       std::pow(C47, 1. / 6.) *
                                                       std::exp(-0.00081290825 * std::sqrt(C57) /
                                                                (std::pow(C47, 7. / 6.) * C81)) *
                                                       0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                                       std::pow(C47, 1. / 6.) /
                                                       (6. * std::pow(std::pow(C47, 7. / 6.) * C81,
                                                                      2))) +
                                              C57 * C81 *
                                                  std::exp(-0.00081290825 * std::sqrt(C57) /
                                                           (std::pow(C47, 7. / 6.) * C81)) *
                                                  0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                                  std::pow(C47, 1. / 6.) *
                                                  std::sqrt(
                                                      (std::pow(C59, C52) + std::pow(C60, C52)) /
                                                      2.) *
                                                  7. * std::pow(C47, 4. / 3.) / 3.) -
                                         std::pow(C47, 7. / 3.) *
                                             std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                       2.) *
                                             C57 * C81 *
                                             std::exp(-0.00081290825 * std::sqrt(C57) /
                                                      (std::pow(C47, 7. / 6.) * C81)) *
                                             0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                             std::pow(C47, 1. / 6.) * 6. * std::pow(C47, 7. / 6.) *
                                             C81 * 2 * C81 * 7. * std::pow(C47, 1. / 6.) / 6.) /
                                            std::pow(6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2),
                                                     2) -
                                        (std::exp(-0.00081290825 * std::sqrt(C57) /
                                                  (std::pow(C47, 7. / 6.) * C81)) *
                                             C81 * C57 *
                                             std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                       2.) *
                                             28. * std::pow(C47, 1. / 3.) / 3. +
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                             7. * std::pow(C47, 4. / 3.) * C57 * C81 *
                                             std::exp(-0.00081290825 * std::sqrt(C57) /
                                                      (std::pow(C47, 7. / 6.) * C81)) *
                                             0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                             std::pow(C47, 1. / 6.) /
                                             (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) /
                                            3.) +
                                 (std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                      C57 * C81 *
                                      std::exp(-0.00081290825 * std::sqrt(C57) /
                                               (std::pow(C47, 7. / 6.) * C81)) *
                                      0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                      std::pow(C47, 1. / 6.) /
                                      (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                  std::exp(-0.00081290825 * std::sqrt(C57) /
                                           (std::pow(C47, 7. / 6.) * C81)) *
                                      C81 * C57 *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                      7. * std::pow(C47, 4. / 3.) / 3.)) -
                            C47 *
                                (std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     C57 * C81 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) /
                                     (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                     C81 * C57 *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     7. * std::pow(C47, 4. / 3.) / 3.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                std::pow(C47, 4. / 3.) / 3.) /
                               std::pow(
                                   std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   4) +
                           (std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * C57 *
                                C81 *
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) *
                                0.00081290825 * std::sqrt(C57) * C81 * 7. * std::pow(C47, 1. / 6.) /
                                (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                C81 * C57 *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                std::pow(C47, 4. / 3.) / 3.) /
                               std::pow(
                                   std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2)) *
                              C114 +
                          (((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) /
                               (2. * (std::pow(2., C49) - 1.)) -
                           1) *
                              C139) +
                         C124 * ((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) * 0.0843 *
                             (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                             (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                              (std::pow(2., C49) - 1.))) +
                        (((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                              std::pow(C59, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) *
                              C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                              (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                               std::pow(
                                   std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2))) -
                         C47 *
                             (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                  std::pow(
                                      std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2) *
                                  (std::exp(-0.00081290825 * std::sqrt(C57) /
                                            (std::pow(C47, 7. / 6.) * C81)) *
                                       C81 * C57 * C52 * std::pow(C59, C52 - 1) * 7. *
                                       std::pow(C47, 4. / 3.) / 3. +
                                   std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * C57 *
                                       C81 *
                                       std::exp(-0.00081290825 * std::sqrt(C57) /
                                                (std::pow(C47, 7. / 6.) * C81)) *
                                       0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                       std::pow(C47, 1. / 6.) /
                                       (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) -
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                                  C81 * C57 * std::pow(C47, 7. / 3.) * C52 *
                                  std::pow(C59, C52 - 1) * 4. *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                  std::pow(C47, 7. / 3.) *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                  std::pow(C47, 4. / 3.) / 3.) /
                             std::pow(
                                 4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     std::pow(
                                         std::pow(C47, 7. / 3.) *
                                             std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                       2.),
                                         2),
                                 2)) *
                            C120) +
                       (((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                             std::pow(C60, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                             C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                             (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                              std::pow(
                                  std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  2))) -
                        C47 *
                            (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                 std::pow(
                                     std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                 (std::exp(-0.00081290825 * std::sqrt(C57) /
                                           (std::pow(C47, 7. / 6.) * C81)) *
                                      C81 * C57 * C52 * std::pow(C60, C52 - 1) * 7. *
                                      std::pow(C47, 4. / 3.) / 3. +
                                  std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * C57 *
                                      C81 *
                                      std::exp(-0.00081290825 * std::sqrt(C57) /
                                               (std::pow(C47, 7. / 6.) * C81)) *
                                      0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                      std::pow(C47, 1. / 6.) /
                                      (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) -
                             std::exp(-0.00081290825 * std::sqrt(C57) /
                                      (std::pow(C47, 7. / 6.) * C81)) *
                                 C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) *
                                 4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                 std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                 std::pow(C47, 4. / 3.) / 3.) /
                            std::pow(
                                4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                    std::pow(
                                        std::pow(C47, 7. / 3.) *
                                            std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                      2.),
                                        2),
                                2)) *
                           C121) +
                      (std::pow(C47, 7. / 3.) *
                           std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * C141 *
                           (C47 *
                                (std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) /
                                     (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                                 (std::pow(std::pow(C47, 7. / 6.) * C81, 2) * C81 *
                                      (std::exp(-0.00081290825 * std::sqrt(C57) /
                                                (std::pow(C47, 7. / 6.) * C81)) *
                                           0.00081290825 * std::sqrt(C57) * 7. *
                                           std::pow(C47, 1. / 6.) / 6. +
                                       0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                                    (std::pow(C47, 7. / 6.) * C81)) *
                                           0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                           std::pow(C47, 1. / 6.) /
                                           (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2))) -
                                  C81 *
                                      std::exp(-0.00081290825 * std::sqrt(C57) /
                                               (std::pow(C47, 7. / 6.) * C81)) *
                                      0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                                      std::pow(C47, 7. / 6.) * C81 * 2 * C81 * 7. *
                                      std::pow(C47, 1. / 6.) / 6.) /
                                     std::pow(std::pow(C47, 7. / 6.) * C81, 4)) *
                                C57 +
                            (std::exp(-0.00081290825 * std::sqrt(C57) /
                                      (std::pow(C47, 7. / 6.) * C81)) +
                             C81 *
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                 std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                                C57) -
                       C141 * C47 *
                           (std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) +
                            C81 *
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) *
                                0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                           C57 * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                           std::pow(C47, 4. / 3.) / 3.) /
                          std::pow(std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2)) *
                         C114 +
                     (((((((std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                C47 *
                                (C57 * C81 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) * std::pow(C47, 7. / 3.) * C52 *
                                     std::pow(C59, C52 - 1) /
                                     (4. *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                      6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                     C81 * C57 * 7. * std::pow(C47, 4. / 3.) * C52 *
                                     std::pow(C59, C52 - 1) /
                                     (12. *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) -
                            C47 *
                                (std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     C57 * C81 *
                                     std::exp(-0.00081290825 * std::sqrt(C57) /
                                              (std::pow(C47, 7. / 6.) * C81)) *
                                     0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                     std::pow(C47, 1. / 6.) /
                                     (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                     C81 * C57 *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     7. * std::pow(C47, 4. / 3.) / 3.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                                (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                               std::pow(
                                   std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   4) +
                           ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                                std::pow(C59, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                                (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                 std::pow(
                                     std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2)))) *
                              C114 +
                          C139 * C47 * C51 * std::pow(C59, C51 - 1) /
                              (2. * (std::pow(2., C49) - 1.))) +
                         C124 * C47 * C51 * std::pow(C59, C51 - 1) * 0.0843 *
                             (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                             (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                              (std::pow(2., C49) - 1.))) +
                        C47 *
                            ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) *
                                 C51 * (C51 - 1) * std::pow(C59, C51 - 2) /
                                 (2. * (std::pow(2., C49) - 1.)) -
                             (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                  std::pow(
                                      std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2) *
                                  std::exp(-0.00081290825 * std::sqrt(C57) /
                                           (std::pow(C47, 7. / 6.) * C81)) *
                                  C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                                  std::pow(C59, C52 - 2) -
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                                  C81 * C57 * std::pow(C47, 7. / 3.) * C52 *
                                  std::pow(C59, C52 - 1) * 4. *
                                  (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                       std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                       2 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                                   std::pow(std::pow(C47, 7. / 3.) *
                                                std::sqrt(
                                                    (std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                            2) *
                                       C52 * std::pow(C59, C52 - 1)) /
                                  (4. *
                                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                                 std::pow(
                                     4. *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                         std::pow(std::pow(C47, 7. / 3.) *
                                                      std::sqrt((std::pow(C59, C52) +
                                                                 std::pow(C60, C52)) /
                                                                2.),
                                                  2),
                                     2)) *
                            C120) +
                       C121 * C47 *
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                           C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                           (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                C52 * std::pow(C59, C52 - 1)) /
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                            std::pow(
                                4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                    std::pow(
                                        std::pow(C47, 7. / 3.) *
                                            std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                      2.),
                                        2),
                                2))) -
                      C141 * C47 *
                          (std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) +
                           C81 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) *
                               0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                               std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                          C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                          (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                           std::pow(std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2))) *
                         C120) +
                    (((((((std::pow(std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) *
                               C47 *
                               (C57 * C81 *
                                    std::exp(-0.00081290825 * std::sqrt(C57) /
                                             (std::pow(C47, 7. / 6.) * C81)) *
                                    0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                    std::pow(C47, 1. / 6.) * std::pow(C47, 7. / 3.) * C52 *
                                    std::pow(C60, C52 - 1) /
                                    (4. *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) *
                                    C81 * C57 * 7. * std::pow(C47, 4. / 3.) * C52 *
                                    std::pow(C60, C52 - 1) /
                                    (12. *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) -
                           C47 *
                               (std::pow(C47, 7. / 3.) *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                    C57 * C81 *
                                    std::exp(-0.00081290825 * std::sqrt(C57) /
                                             (std::pow(C47, 7. / 6.) * C81)) *
                                    0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                    std::pow(C47, 1. / 6.) /
                                    (6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2)) -
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) *
                                    C81 * C57 *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 7. *
                                    std::pow(C47, 4. / 3.) / 3.) *
                               std::pow(C47, 7. / 3.) *
                               std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                               std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                               (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                              std::pow(
                                  std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  4) +
                          ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                               std::pow(C60, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                               C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                               (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(
                                    std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2)))) *
                             C114 +
                         C139 * C47 * C51 * std::pow(C60, C51 - 1) /
                             (2. * (std::pow(2., C49) - 1.))) +
                        C124 * C47 * C51 * std::pow(C60, C51 - 1) * 0.0843 *
                            (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                            (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                             (std::pow(2., C49) - 1.))) +
                       C120 * C47 *
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                           C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * 4. *
                           (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                C52 * std::pow(C60, C52 - 1)) /
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                            std::pow(
                                4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                    std::pow(
                                        std::pow(C47, 7. / 3.) *
                                            std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                      2.),
                                        2),
                                2))) +
                      C47 *
                          ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                               (C51 - 1) * std::pow(C60, C51 - 2) /
                               (2. * (std::pow(2., C49) - 1.)) -
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(
                                    std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) *
                                std::exp(-0.00081290825 * std::sqrt(C57) /
                                         (std::pow(C47, 7. / 6.) * C81)) *
                                C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                                std::pow(C60, C52 - 2) -
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                                C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) *
                                4. *
                                (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                     std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                     std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                                 std::pow(
                                     std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                     C52 * std::pow(C60, C52 - 1)) /
                                (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                               std::pow(
                                   4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                       std::pow(
                                           std::pow(C47, 7. / 3.) *
                                               std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) /
                                                         2.),
                                           2),
                                   2)) *
                          C121) -
                     C141 * C47 *
                         (std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) +
                          C81 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                              std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                         C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                         (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                          std::pow(std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2))) *
                        C121) +
                   ((((C47 *
                           ((6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2) *
                                 std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * C57 *
                                 (C81 *
                                      (std::exp(-0.00081290825 * std::sqrt(C57) /
                                                (std::pow(C47, 7. / 6.) * C81)) *
                                           0.00081290825 * std::sqrt(C57) * 7. *
                                           std::pow(C47, 1. / 6.) +
                                       0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                           std::pow(C47, 1. / 6.) *
                                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                                    (std::pow(C47, 7. / 6.) * C81)) *
                                           0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                           std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                                  std::exp(-0.00081290825 * std::sqrt(C57) /
                                           (std::pow(C47, 7. / 6.) * C81)) *
                                      0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                      std::pow(C47, 1. / 6.)) -
                             std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * C57 *
                                 C81 *
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                 0.00081290825 * std::sqrt(C57) * C81 * 7. *
                                 std::pow(C47, 1. / 6.) * 12. * C81 * std::pow(C47, 14. / 6.)) /
                                std::pow(6. * std::pow(std::pow(C47, 7. / 6.) * C81, 2), 2) -
                            (std::exp(-0.00081290825 * std::sqrt(C57) /
                                      (std::pow(C47, 7. / 6.) * C81)) +
                             C81 *
                                 std::exp(-0.00081290825 * std::sqrt(C57) /
                                          (std::pow(C47, 7. / 6.) * C81)) *
                                 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                 std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                                C57 * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                7. * std::pow(C47, 4. / 3.) / 3.) /
                           std::pow(std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) +
                       (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                        C81 *
                            std::exp(-0.00081290825 * std::sqrt(C57) /
                                     (std::pow(C47, 7. / 6.) * C81)) *
                            0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                            std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                           C57 /
                           (std::pow(C47, 7. / 3.) *
                            std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                          C114 -
                      C120 * C47 *
                          (std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) +
                           C81 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) *
                               0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                               std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                          C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                          (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                           std::pow(std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2))) -
                     C121 * C47 *
                         (std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) +
                          C81 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                              std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                         C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                         (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                          std::pow(std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2))) +
                    C141 * C47 *
                        (std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                             0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                             std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                         (std::pow(std::pow(C47, 7. / 6.) * C81, 2) *
                              (C81 * 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                                   std::exp(-0.00081290825 * std::sqrt(C57) /
                                            (std::pow(C47, 7. / 6.) * C81)) *
                                   0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                                   std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) *
                                   0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) -
                          C81 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * 2 * C81 *
                              std::pow(C47, 14. / 6.)) /
                             std::pow(std::pow(C47, 7. / 6.) * C81, 4)) *
                        C57 /
                        (std::pow(C47, 7. / 3.) *
                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                       C141) +
                  (((((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) /
                         (2. * (std::pow(2., C49) - 1.)) -
                     1) *
                        C114 +
                    C120 * C47 * C51 * std::pow(C59, C51 - 1) / (2. * (std::pow(2., C49) - 1.))) +
                   C121 * C47 * C51 * std::pow(C60, C51 - 1) / (2. * (std::pow(2., C49) - 1.))) *
                      C139) +
                 (((C114 * ((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) * 0.0843 *
                        (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                        (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                         (std::pow(2., C49) - 1.)) +
                    (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                         (std::pow(2., C49) - 1.) * C124 * C47 * -0.11785983 *
                         ((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) /
                         (std::sqrt(C63) * std::pow(2 * std::sqrt(C63), 2)) -
                     C124 * C47 * ((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) * 0.0843 *
                         (1.3981 / (2 * std::sqrt(C63)) + 0.2611) * 2 *
                         (1.3981 / (2 * std::sqrt(C63)) + 0.2611) *
                         ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.) * 2. *
                         (std::pow(2., C49) - 1.)) /
                        std::pow(std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                                     (std::pow(2., C49) - 1.),
                                 2)) +
                   C120 * C47 * C51 * std::pow(C59, C51 - 1) * 0.0843 *
                       (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                       (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                        (std::pow(2., C49) - 1.))) +
                  C121 * C47 * C51 * std::pow(C60, C51 - 1) * 0.0843 *
                      (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                      (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                       (std::pow(2., C49) - 1.))) *
                     C124) +
                C47 *
                    (((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) /
                         (2. * (std::pow(2., C49) - 1.)) -
                     1) *
                    C460) +
               C445 * C47 * ((std::pow(C59, C51) + std::pow(C60, C51)) - 2.) * 0.0843 *
                   (1.3981 / (2 * std::sqrt(C63)) + 0.2611) /
                   (std::pow((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1., 2) * 2. *
                    (std::pow(2., C49) - 1.))) +
              C47 *
                  ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                       std::pow(C59, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                   std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                       C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                       (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                        std::pow(std::pow(C47, 7. / 3.) *
                                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                 2))) *
                  C441) +
             C47 *
                 ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                      std::pow(C60, C51 - 1) / (2. * (std::pow(2., C49) - 1.)) -
                  std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) * C81 *
                      C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                       std::pow(std::pow(C47, 7. / 3.) *
                                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                2))) *
                 C442) +
            C462 * C47 *
                (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                 C81 * std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                     0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                     std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                C57 /
                (std::pow(C47, 7. / 3.) *
                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.));

        const double C463 = -24. * Pi;
        const double C464 = 0.88668 * Pi;
        const double C465 = 2. * sigmaAb;
        const double C466 = 12. * Pi;
        const double C467 = rhoA + rhoB;
        const double C468 = rhoA - rhoB;
        const double C469 = -2. / 3.;
        const double C470 = -1. / 3.;
        const double C471 = 1. / 3.;
        const double C472 = 2. / 3.;
        const double C473 = 4. / 3.;
        const double C474 = 5. / 3.;
        const double C475 = 7. / 6.;
        const double C476 = -0.9999999999999999 * C467;
        const double C477 = 0.9999999999999999 * C468;
        const double C478 = Pi * C467;
        const double C479 = sigmaAa + C465;
        const double C480 = std::pow(C467, C473);
        const double C481 = std::pow(C467, C475);
        const double C482 = 4. * C478;
        const double C483 = C479 + sigmaBb;
        const double C484 = C476 - C477;
        const double C485 = C477 / C467;
        const double C486 = C485 + 1.;
        const double C487 = 1. - C485;
        const double C488 = 0.22167 / C482;
        const double C489 = 3. / C482;
        const double C490 = std::sqrt(C483);
        const double C491 = std::pow(C482, 2);
        const double C492 = 0.00081290825 * C490;
        const double C493 = 3. * C491;
        const double C494 = C464 / C491;
        const double C495 = std::pow(C486, C474);
        const double C496 = std::pow(C487, C474);
        const double C497 = std::pow(C489, C469);
        const double C498 = std::pow(C489, C470);
        const double C499 = std::pow(C489, C471);
        const double C500 = std::pow(C489, C472);
        const double C501 = 7.389e-6 * C500;
        const double C502 = 0.023266 * C499;
        const double C503 = 0.472 * C500;
        const double C504 = 8.723 * C499;
        const double C505 = C497 * C466;
        const double C506 = C498 * C463;
        const double C507 = C495 + C496;
        const double C508 = -8.723 * C505;
        const double C509 = -0.023266 * C505;
        const double C510 = 7.389e-6 * C506;
        const double C511 = 0.472 * C506;
        const double C512 = C502 + C501;
        const double C513 = C504 + C503;
        const double C514 = C507 / 2.;
        const double C515 = C508 + C511;
        const double C516 = C509 + C510;
        const double C517 = C512 + 0.002568;
        const double C518 = C513 + C488;
        const double C519 = std::sqrt(C514);
        const double C520 = C480 * C519;
        const double C521 = C518 + 1.;
        const double C522 = C515 / C493;
        const double C523 = C521 * C516;
        const double C524 = C522 - C494;
        const double C525 = C517 / C521;
        const double C526 = std::pow(C521, 2);
        const double C527 = C517 * C524;
        const double C528 = C525 + 0.001667;
        const double C529 = C523 / C493;
        const double C530 = C528 * C481;
        const double C531 = C529 - C527;
        const double C532 = C492 / C530;
        const double C533 = -C532;
        const double C534 = std::exp(C533);
        matrix.upper[6] =
            C534 * (1. - C492 / (2. * C530)) *
                (C520 * C531 / C526 - C528 * (C480 *
                                                  (std::pow(C486, C472) * 5. * C484 +
                                                   std::pow(C487, C472) * -5. * C484) /
                                                  (6. * std::pow(C467, 2) * 2 * C519) +
                                              C519 * 4. * std::pow(C467, C471) / 3.)) /
                std::pow(C520, 2) +
            C528 * 0.00081290825 * (C528 * 7. * std::pow(C467, 1. / 6.) / 6. + C481 * C531 / C526) *
                (3. - C532) * C534 * C490 / (2. * std::pow(C530, 2) * C520);

        const double C535 = -24. * Pi;
        const double C536 = 0.88668 * Pi;
        const double C537 = 2. * sigmaAb;
        const double C538 = 12. * Pi;
        const double C539 = rhoA + rhoB;
        const double C540 = rhoA - rhoB;
        const double C541 = -2. / 3.;
        const double C542 = -1. / 3.;
        const double C543 = 1. / 3.;
        const double C544 = 2. / 3.;
        const double C545 = 4. / 3.;
        const double C546 = 5. / 3.;
        const double C547 = 7. / 6.;
        const double C548 = -0.9999999999999999 * C539;
        const double C549 = 0.9999999999999999 * C540;
        const double C550 = Pi * C539;
        const double C551 = sigmaAa + C537;
        const double C552 = std::pow(C539, C545);
        const double C553 = std::pow(C539, C547);
        const double C554 = 4. * C550;
        const double C555 = C551 + sigmaBb;
        const double C556 = C548 - C549;
        const double C557 = C549 / C539;
        const double C558 = C557 + 1.;
        const double C559 = 1. - C557;
        const double C560 = 0.22167 / C554;
        const double C561 = 3. / C554;
        const double C562 = std::sqrt(C555);
        const double C563 = std::pow(C554, 2);
        const double C564 = 0.00081290825 * C562;
        const double C565 = 3. * C563;
        const double C566 = C536 / C563;
        const double C567 = std::pow(C558, C546);
        const double C568 = std::pow(C559, C546);
        const double C569 = std::pow(C561, C541);
        const double C570 = std::pow(C561, C542);
        const double C571 = std::pow(C561, C543);
        const double C572 = std::pow(C561, C544);
        const double C573 = 7.389e-6 * C572;
        const double C574 = 0.023266 * C571;
        const double C575 = 0.472 * C572;
        const double C576 = 8.723 * C571;
        const double C577 = C569 * C538;
        const double C578 = C570 * C535;
        const double C579 = C567 + C568;
        const double C580 = -8.723 * C577;
        const double C581 = -0.023266 * C577;
        const double C582 = 7.389e-6 * C578;
        const double C583 = 0.472 * C578;
        const double C584 = C574 + C573;
        const double C585 = C576 + C575;
        const double C586 = C579 / 2.;
        const double C587 = C580 + C583;
        const double C588 = C581 + C582;
        const double C589 = C584 + 0.002568;
        const double C590 = C585 + C560;
        const double C591 = std::sqrt(C586);
        const double C592 = C552 * C591;
        const double C593 = C590 + 1.;
        const double C594 = C587 / C565;
        const double C595 = C593 * C588;
        const double C596 = C594 - C566;
        const double C597 = C589 / C593;
        const double C598 = std::pow(C593, 2);
        const double C599 = C589 * C596;
        const double C600 = C597 + 0.001667;
        const double C601 = C595 / C565;
        const double C602 = C600 * C553;
        const double C603 = C601 - C599;
        const double C604 = C564 / C602;
        const double C605 = -C604;
        const double C606 = std::exp(C605);
        matrix.upper[7] =
            2. * (C606 * (1. - C564 / (2. * C602)) *
                      (C592 * C603 / C598 - C600 * (C552 *
                                                        (std::pow(C558, C544) * 5. * C556 +
                                                         std::pow(C559, C544) * -5. * C556) /
                                                        (6. * std::pow(C539, 2) * 2 * C591) +
                                                    C591 * 4. * std::pow(C539, C543) / 3.)) /
                      std::pow(C592, 2) +
                  C600 * 0.00081290825 *
                      (C600 * 7. * std::pow(C539, 1. / 6.) / 6. + C553 * C603 / C598) *
                      (3. - C604) * C606 * C562 / (2. * std::pow(C602, 2) * C592));

        const double C607 = -24. * Pi;
        const double C608 = 0.88668 * Pi;
        const double C609 = 2. * sigmaAb;
        const double C610 = 12. * Pi;
        const double C611 = rhoA + rhoB;
        const double C612 = rhoA - rhoB;
        const double C613 = -2. / 3.;
        const double C614 = -1. / 3.;
        const double C615 = 1. / 3.;
        const double C616 = 2. / 3.;
        const double C617 = 4. / 3.;
        const double C618 = 5. / 3.;
        const double C619 = 7. / 6.;
        const double C620 = -0.9999999999999999 * C611;
        const double C621 = 0.9999999999999999 * C612;
        const double C622 = Pi * C611;
        const double C623 = sigmaAa + C609;
        const double C624 = std::pow(C611, C617);
        const double C625 = std::pow(C611, C619);
        const double C626 = 4. * C622;
        const double C627 = C623 + sigmaBb;
        const double C628 = C620 - C621;
        const double C629 = C621 / C611;
        const double C630 = C629 + 1.;
        const double C631 = 1. - C629;
        const double C632 = 0.22167 / C626;
        const double C633 = 3. / C626;
        const double C634 = std::sqrt(C627);
        const double C635 = std::pow(C626, 2);
        const double C636 = 0.00081290825 * C634;
        const double C637 = 3. * C635;
        const double C638 = C608 / C635;
        const double C639 = std::pow(C630, C618);
        const double C640 = std::pow(C631, C618);
        const double C641 = std::pow(C633, C613);
        const double C642 = std::pow(C633, C614);
        const double C643 = std::pow(C633, C615);
        const double C644 = std::pow(C633, C616);
        const double C645 = 7.389e-6 * C644;
        const double C646 = 0.023266 * C643;
        const double C647 = 0.472 * C644;
        const double C648 = 8.723 * C643;
        const double C649 = C641 * C610;
        const double C650 = C642 * C607;
        const double C651 = C639 + C640;
        const double C652 = -8.723 * C649;
        const double C653 = -0.023266 * C649;
        const double C654 = 7.389e-6 * C650;
        const double C655 = 0.472 * C650;
        const double C656 = C646 + C645;
        const double C657 = C648 + C647;
        const double C658 = C651 / 2.;
        const double C659 = C652 + C655;
        const double C660 = C653 + C654;
        const double C661 = C656 + 0.002568;
        const double C662 = C657 + C632;
        const double C663 = std::sqrt(C658);
        const double C664 = C624 * C663;
        const double C665 = C662 + 1.;
        const double C666 = C659 / C637;
        const double C667 = C665 * C660;
        const double C668 = C666 - C638;
        const double C669 = C661 / C665;
        const double C670 = std::pow(C665, 2);
        const double C671 = C661 * C668;
        const double C672 = C669 + 0.001667;
        const double C673 = C667 / C637;
        const double C674 = C672 * C625;
        const double C675 = C673 - C671;
        const double C676 = C636 / C674;
        const double C677 = -C676;
        const double C678 = std::exp(C677);
        matrix.upper[8] =
            C678 * (1. - C636 / (2. * C674)) *
                (C664 * C675 / C670 - C672 * (C624 *
                                                  (std::pow(C630, C616) * 5. * C628 +
                                                   std::pow(C631, C616) * -5. * C628) /
                                                  (6. * std::pow(C611, 2) * 2 * C663) +
                                              C663 * 4. * std::pow(C611, C615) / 3.)) /
                std::pow(C664, 2) +
            C672 * 0.00081290825 * (C672 * 7. * std::pow(C611, 1. / 6.) / 6. + C625 * C675 / C670) *
                (3. - C676) * C678 * C634 / (2. * std::pow(C674, 2) * C664);

        const double C142 = 0.;
        const double C143 = 1;
        const double C144 = C143;
        const double C145 = C142 / C47;
        const double C146 = C145;
        const double C147 = -C145;
        const double C148 = 0.;
        const double C149 = C148;
        const double C150 = C149;
        const double C151 = C150 / C78;
        const double C152 = C151;
        matrix.upper[9] =
            (((((C47 *
                     ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                          (C51 - 1) * std::pow(C59, C51 - 2) / (2. * (std::pow(2., C49) - 1.)) -
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                           std::pow(std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) *
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                           C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                           std::pow(C59, C52 - 2) -
                       std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                           C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * 4. *
                           (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                C52 * std::pow(C59, C52 - 1)) /
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                          std::pow(
                              4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                  std::pow(
                                      std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2),
                              2)) *
                     C146 +
                 C147 * C47 *
                     std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                     C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                     (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                          std::pow(C47, 7. / 3.) *
                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                          std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                      std::pow(std::pow(C47, 7. / 3.) *
                                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                               2) *
                          C52 * std::pow(C59, C52 - 1)) /
                     (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                      std::pow(4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                   std::pow(std::pow(C47, 7. / 3.) *
                                                std::sqrt(
                                                    (std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                            2),
                               2))) -
                C144 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                         C81 +
                     C57 * C81 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) /
                         (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                    std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                     std::pow(std::pow(C47, 7. / 3.) *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                              2))) -
               C152 * C47 *
                   (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                    C81 *
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                        0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                        std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                   C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                   (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                    std::pow(std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                             2))) *
                  C146 +
              ((C144 * C47 *
                    (C81 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) /
                         (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81) +
                     (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81 *
                          (C57 * C81 * 0.0000006608198229 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) /
                               (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81) +
                           C81 * -0.00081290825 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81))) -
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) *
                          std::pow(C47, 7. / 6.) * C81 / std::sqrt(C57)) /
                         std::pow(2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81, 2)) /
                    (std::pow(C47, 7. / 3.) *
                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.)) -
                (C147 * C47 *
                     (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                          C81 +
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) /
                          (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                     std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                 C146 * C47 *
                     (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                          C81 +
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) /
                          (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                     std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1)) /
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                     std::pow(std::pow(C47, 7. / 3.) *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                              2))) +
               C152 * C47 *
                   ((std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                    (-0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) /
                         (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81) +
                     C81 *
                         (std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::pow(C47, 7. / 6.) / (2 * std::sqrt(C57)) +
                          0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) /
                              (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                        C57) /
                   (std::pow(C47, 7. / 3.) *
                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                  C144) +
             ((C144 * C47 *
                   ((std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                    (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81 * C57 *
                         (C81 * -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                              std::pow(std::pow(C47, 7. / 6.) * C81, 2) -
                          0.00081290825 * std::exp(-0.00081290825 * std::sqrt(C57) /
                                                   (std::pow(C47, 7. / 6.) * C81))) -
                     C57 * C81 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         2 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) /
                        std::pow(2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81, 2)) /
                   (std::pow(C47, 7. / 3.) *
                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.)) -
               (C147 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                    C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                C146 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                    C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1)) /
                   (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                    std::pow(std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                             2))) +
              C152 * C47 *
                  (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                       0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                       std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                   (std::pow(std::pow(C47, 7. / 6.) * C81, 2) *
                        (C81 * 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                             std::exp(-0.00081290825 * std::sqrt(C57) /
                                      (std::pow(C47, 7. / 6.) * C81)) *
                             0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                             std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                             0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) -
                    C81 *
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                        0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * 2 * C81 *
                        std::pow(C47, 14. / 6.)) /
                       std::pow(std::pow(C47, 7. / 6.) * C81, 4)) *
                  C57 /
                  (std::pow(C47, 7. / 3.) *
                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                 C152) +
            (((C146 * C47 *
                   std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                   C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * 4. *
                   (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                        std::pow(C47, 7. / 3.) *
                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                        std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                    std::pow(std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                             2) *
                        C52 * std::pow(C60, C52 - 1)) /
                   (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                    std::pow(
                        4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2),
                        2)) +
               C47 *
                   ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                        (C51 - 1) * std::pow(C60, C51 - 2) / (2. * (std::pow(2., C49) - 1.)) -
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                         std::pow(std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  2) *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                         std::pow(C60, C52 - 2) -
                     std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                         C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                         (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                              std::pow(C47, 7. / 3.) *
                              std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                              std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                          std::pow(std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2) *
                              C52 * std::pow(C60, C52 - 1)) /
                         (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                        std::pow(
                            4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(
                                    std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2),
                            2)) *
                   C147) -
              C144 * C47 *
                  (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                       C81 +
                   C57 * C81 * -0.00081290825 *
                       std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) /
                       (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                  std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                  (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                   std::pow(std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                            2))) -
             C152 * C47 *
                 (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                  C81 * std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                      0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                      std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                 C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                 (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                  std::pow(std::pow(C47, 7. / 3.) *
                               std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                           2))) *
                C147;

        const double C153 = 2.;
        const double C154 = 0.;
        const double C155 = C153;
        const double C156 = C155;
        const double C157 = C154 / C47;
        const double C158 = C157;
        const double C159 = -C157;
        const double C160 = 0.;
        const double C161 = C160;
        const double C162 = C161;
        const double C163 = C162 / C78;
        const double C164 = C163;
        matrix.upper[10] =
            (((((C47 *
                     ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                          (C51 - 1) * std::pow(C59, C51 - 2) / (2. * (std::pow(2., C49) - 1.)) -
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                           std::pow(std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) *
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                           C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                           std::pow(C59, C52 - 2) -
                       std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                           C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * 4. *
                           (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                C52 * std::pow(C59, C52 - 1)) /
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                          std::pow(
                              4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                  std::pow(
                                      std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2),
                              2)) *
                     C146 +
                 C147 * C47 *
                     std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                     C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                     (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                          std::pow(C47, 7. / 3.) *
                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                          std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                      std::pow(std::pow(C47, 7. / 3.) *
                                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                               2) *
                          C52 * std::pow(C59, C52 - 1)) /
                     (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                      std::pow(4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                   std::pow(std::pow(C47, 7. / 3.) *
                                                std::sqrt(
                                                    (std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                            2),
                               2))) -
                C144 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                         C81 +
                     C57 * C81 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) /
                         (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                    std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                     std::pow(std::pow(C47, 7. / 3.) *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                              2))) -
               C152 * C47 *
                   (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                    C81 *
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                        0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                        std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                   C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                   (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                    std::pow(std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                             2))) *
                  C158 +
              ((C144 * C47 *
                    (C81 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) /
                         (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81) +
                     (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81 *
                          (C57 * C81 * 0.0000006608198229 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) /
                               (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81) +
                           C81 * -0.00081290825 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81))) -
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) *
                          std::pow(C47, 7. / 6.) * C81 / std::sqrt(C57)) /
                         std::pow(2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81, 2)) /
                    (std::pow(C47, 7. / 3.) *
                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.)) -
                (C147 * C47 *
                     (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                          C81 +
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) /
                          (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                     std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                 C146 * C47 *
                     (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                          C81 +
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) /
                          (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                     std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1)) /
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                     std::pow(std::pow(C47, 7. / 3.) *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                              2))) +
               C152 * C47 *
                   ((std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                    (-0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) /
                         (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81) +
                     C81 *
                         (std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::pow(C47, 7. / 6.) / (2 * std::sqrt(C57)) +
                          0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) /
                              (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                        C57) /
                   (std::pow(C47, 7. / 3.) *
                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                  C156) +
             ((C144 * C47 *
                   ((std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                    (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81 * C57 *
                         (C81 * -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                              std::pow(std::pow(C47, 7. / 6.) * C81, 2) -
                          0.00081290825 * std::exp(-0.00081290825 * std::sqrt(C57) /
                                                   (std::pow(C47, 7. / 6.) * C81))) -
                     C57 * C81 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         2 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) /
                        std::pow(2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81, 2)) /
                   (std::pow(C47, 7. / 3.) *
                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.)) -
               (C147 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                    C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                C146 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                    C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1)) /
                   (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                    std::pow(std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                             2))) +
              C152 * C47 *
                  (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                       0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                       std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                   (std::pow(std::pow(C47, 7. / 6.) * C81, 2) *
                        (C81 * 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                             std::exp(-0.00081290825 * std::sqrt(C57) /
                                      (std::pow(C47, 7. / 6.) * C81)) *
                             0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                             std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                             0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) -
                    C81 *
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                        0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * 2 * C81 *
                        std::pow(C47, 14. / 6.)) /
                       std::pow(std::pow(C47, 7. / 6.) * C81, 4)) *
                  C57 /
                  (std::pow(C47, 7. / 3.) *
                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                 C164) +
            (((C146 * C47 *
                   std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                   C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * 4. *
                   (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                        std::pow(C47, 7. / 3.) *
                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                        std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                    std::pow(std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                             2) *
                        C52 * std::pow(C60, C52 - 1)) /
                   (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                    std::pow(
                        4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2),
                        2)) +
               C47 *
                   ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                        (C51 - 1) * std::pow(C60, C51 - 2) / (2. * (std::pow(2., C49) - 1.)) -
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                         std::pow(std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  2) *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                         std::pow(C60, C52 - 2) -
                     std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                         C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                         (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                              std::pow(C47, 7. / 3.) *
                              std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                              std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                          std::pow(std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2) *
                              C52 * std::pow(C60, C52 - 1)) /
                         (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                        std::pow(
                            4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(
                                    std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2),
                            2)) *
                   C147) -
              C144 * C47 *
                  (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                       C81 +
                   C57 * C81 * -0.00081290825 *
                       std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) /
                       (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                  std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                  (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                   std::pow(std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                            2))) -
             C152 * C47 *
                 (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                  C81 * std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                      0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                      std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                 C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                 (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                  std::pow(std::pow(C47, 7. / 3.) *
                               std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                           2))) *
                C159;

        const double C165 = 0.;
        const double C166 = 1;
        const double C167 = C165 / C47;
        const double C168 = C167;
        const double C169 = -C167;
        const double C170 = 0.;
        const double C171 = C170;
        const double C172 = C171;
        const double C173 = C172 / C78;
        const double C174 = C173;
        matrix.upper[11] =
            (((((C47 *
                     ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                          (C51 - 1) * std::pow(C59, C51 - 2) / (2. * (std::pow(2., C49) - 1.)) -
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                           std::pow(std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) *
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                           C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                           std::pow(C59, C52 - 2) -
                       std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                           C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * 4. *
                           (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                C52 * std::pow(C59, C52 - 1)) /
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                          std::pow(
                              4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                  std::pow(
                                      std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2),
                              2)) *
                     C146 +
                 C147 * C47 *
                     std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                     C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                     (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                          std::pow(C47, 7. / 3.) *
                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                          std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                      std::pow(std::pow(C47, 7. / 3.) *
                                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                               2) *
                          C52 * std::pow(C59, C52 - 1)) /
                     (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                      std::pow(4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                   std::pow(std::pow(C47, 7. / 3.) *
                                                std::sqrt(
                                                    (std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                            2),
                               2))) -
                C144 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                         C81 +
                     C57 * C81 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) /
                         (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                    std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                     std::pow(std::pow(C47, 7. / 3.) *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                              2))) -
               C152 * C47 *
                   (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                    C81 *
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                        0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                        std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                   C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                   (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                    std::pow(std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                             2))) *
                  C168 +
              ((C144 * C47 *
                    (C81 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) /
                         (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81) +
                     (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81 *
                          (C57 * C81 * 0.0000006608198229 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) /
                               (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81) +
                           C81 * -0.00081290825 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81))) -
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) *
                          std::pow(C47, 7. / 6.) * C81 / std::sqrt(C57)) /
                         std::pow(2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81, 2)) /
                    (std::pow(C47, 7. / 3.) *
                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.)) -
                (C147 * C47 *
                     (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                          C81 +
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) /
                          (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                     std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                 C146 * C47 *
                     (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                          C81 +
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) /
                          (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                     std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1)) /
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                     std::pow(std::pow(C47, 7. / 3.) *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                              2))) +
               C152 * C47 *
                   ((std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                    (-0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) /
                         (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81) +
                     C81 *
                         (std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::pow(C47, 7. / 6.) / (2 * std::sqrt(C57)) +
                          0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) /
                              (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                        C57) /
                   (std::pow(C47, 7. / 3.) *
                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                  C166) +
             ((C144 * C47 *
                   ((std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                    (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81 * C57 *
                         (C81 * -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                              std::pow(std::pow(C47, 7. / 6.) * C81, 2) -
                          0.00081290825 * std::exp(-0.00081290825 * std::sqrt(C57) /
                                                   (std::pow(C47, 7. / 6.) * C81))) -
                     C57 * C81 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         2 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) /
                        std::pow(2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81, 2)) /
                   (std::pow(C47, 7. / 3.) *
                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.)) -
               (C147 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                    C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                C146 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                    C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1)) /
                   (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                    std::pow(std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                             2))) +
              C152 * C47 *
                  (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                       0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                       std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                   (std::pow(std::pow(C47, 7. / 6.) * C81, 2) *
                        (C81 * 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                             std::exp(-0.00081290825 * std::sqrt(C57) /
                                      (std::pow(C47, 7. / 6.) * C81)) *
                             0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                             std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                             0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) -
                    C81 *
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                        0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * 2 * C81 *
                        std::pow(C47, 14. / 6.)) /
                       std::pow(std::pow(C47, 7. / 6.) * C81, 4)) *
                  C57 /
                  (std::pow(C47, 7. / 3.) *
                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                 C174) +
            (((C146 * C47 *
                   std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                   C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * 4. *
                   (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                        std::pow(C47, 7. / 3.) *
                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                        std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                    std::pow(std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                             2) *
                        C52 * std::pow(C60, C52 - 1)) /
                   (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                    std::pow(
                        4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2),
                        2)) +
               C47 *
                   ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                        (C51 - 1) * std::pow(C60, C51 - 2) / (2. * (std::pow(2., C49) - 1.)) -
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                         std::pow(std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  2) *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                         std::pow(C60, C52 - 2) -
                     std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                         C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                         (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                              std::pow(C47, 7. / 3.) *
                              std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                              std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                          std::pow(std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2) *
                              C52 * std::pow(C60, C52 - 1)) /
                         (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                        std::pow(
                            4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(
                                    std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2),
                            2)) *
                   C147) -
              C144 * C47 *
                  (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                       C81 +
                   C57 * C81 * -0.00081290825 *
                       std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) /
                       (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                  std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                  (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                   std::pow(std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                            2))) -
             C152 * C47 *
                 (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                  C81 * std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                      0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                      std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                 C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                 (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                  std::pow(std::pow(C47, 7. / 3.) *
                               std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                           2))) *
                C169;

        matrix.upper[12] =
            (((((C47 *
                     ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                          (C51 - 1) * std::pow(C59, C51 - 2) / (2. * (std::pow(2., C49) - 1.)) -
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                           std::pow(std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) *
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                           C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                           std::pow(C59, C52 - 2) -
                       std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                           C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * 4. *
                           (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                C52 * std::pow(C59, C52 - 1)) /
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                          std::pow(
                              4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                  std::pow(
                                      std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2),
                              2)) *
                     C158 +
                 C159 * C47 *
                     std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                     C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                     (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                          std::pow(C47, 7. / 3.) *
                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                          std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                      std::pow(std::pow(C47, 7. / 3.) *
                                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                               2) *
                          C52 * std::pow(C59, C52 - 1)) /
                     (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                      std::pow(4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                   std::pow(std::pow(C47, 7. / 3.) *
                                                std::sqrt(
                                                    (std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                            2),
                               2))) -
                C156 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                         C81 +
                     C57 * C81 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) /
                         (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                    std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                     std::pow(std::pow(C47, 7. / 3.) *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                              2))) -
               C164 * C47 *
                   (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                    C81 *
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                        0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                        std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                   C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                   (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                    std::pow(std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                             2))) *
                  C158 +
              ((C156 * C47 *
                    (C81 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) /
                         (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81) +
                     (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81 *
                          (C57 * C81 * 0.0000006608198229 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) /
                               (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81) +
                           C81 * -0.00081290825 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81))) -
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) *
                          std::pow(C47, 7. / 6.) * C81 / std::sqrt(C57)) /
                         std::pow(2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81, 2)) /
                    (std::pow(C47, 7. / 3.) *
                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.)) -
                (C159 * C47 *
                     (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                          C81 +
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) /
                          (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                     std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                 C158 * C47 *
                     (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                          C81 +
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) /
                          (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                     std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1)) /
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                     std::pow(std::pow(C47, 7. / 3.) *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                              2))) +
               C164 * C47 *
                   ((std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                    (-0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) /
                         (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81) +
                     C81 *
                         (std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::pow(C47, 7. / 6.) / (2 * std::sqrt(C57)) +
                          0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) /
                              (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                        C57) /
                   (std::pow(C47, 7. / 3.) *
                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                  C156) +
             ((C156 * C47 *
                   ((std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                    (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81 * C57 *
                         (C81 * -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                              std::pow(std::pow(C47, 7. / 6.) * C81, 2) -
                          0.00081290825 * std::exp(-0.00081290825 * std::sqrt(C57) /
                                                   (std::pow(C47, 7. / 6.) * C81))) -
                     C57 * C81 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         2 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) /
                        std::pow(2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81, 2)) /
                   (std::pow(C47, 7. / 3.) *
                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.)) -
               (C159 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                    C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                C158 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                    C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1)) /
                   (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                    std::pow(std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                             2))) +
              C164 * C47 *
                  (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                       0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                       std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                   (std::pow(std::pow(C47, 7. / 6.) * C81, 2) *
                        (C81 * 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                             std::exp(-0.00081290825 * std::sqrt(C57) /
                                      (std::pow(C47, 7. / 6.) * C81)) *
                             0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                             std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                             0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) -
                    C81 *
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                        0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * 2 * C81 *
                        std::pow(C47, 14. / 6.)) /
                       std::pow(std::pow(C47, 7. / 6.) * C81, 4)) *
                  C57 /
                  (std::pow(C47, 7. / 3.) *
                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                 C164) +
            (((C158 * C47 *
                   std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                   C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * 4. *
                   (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                        std::pow(C47, 7. / 3.) *
                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                        std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                    std::pow(std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                             2) *
                        C52 * std::pow(C60, C52 - 1)) /
                   (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                    std::pow(
                        4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2),
                        2)) +
               C47 *
                   ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                        (C51 - 1) * std::pow(C60, C51 - 2) / (2. * (std::pow(2., C49) - 1.)) -
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                         std::pow(std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  2) *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                         std::pow(C60, C52 - 2) -
                     std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                         C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                         (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                              std::pow(C47, 7. / 3.) *
                              std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                              std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                          std::pow(std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2) *
                              C52 * std::pow(C60, C52 - 1)) /
                         (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                        std::pow(
                            4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(
                                    std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2),
                            2)) *
                   C159) -
              C156 * C47 *
                  (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                       C81 +
                   C57 * C81 * -0.00081290825 *
                       std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) /
                       (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                  std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                  (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                   std::pow(std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                            2))) -
             C164 * C47 *
                 (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                  C81 * std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                      0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                      std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                 C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                 (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                  std::pow(std::pow(C47, 7. / 3.) *
                               std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                           2))) *
                C159;

        matrix.upper[13] =
            (((((C47 *
                     ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                          (C51 - 1) * std::pow(C59, C51 - 2) / (2. * (std::pow(2., C49) - 1.)) -
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                           std::pow(std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) *
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                           C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                           std::pow(C59, C52 - 2) -
                       std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                           C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * 4. *
                           (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                C52 * std::pow(C59, C52 - 1)) /
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                          std::pow(
                              4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                  std::pow(
                                      std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2),
                              2)) *
                     C158 +
                 C159 * C47 *
                     std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                     C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                     (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                          std::pow(C47, 7. / 3.) *
                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                          std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                      std::pow(std::pow(C47, 7. / 3.) *
                                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                               2) *
                          C52 * std::pow(C59, C52 - 1)) /
                     (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                      std::pow(4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                   std::pow(std::pow(C47, 7. / 3.) *
                                                std::sqrt(
                                                    (std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                            2),
                               2))) -
                C156 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                         C81 +
                     C57 * C81 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) /
                         (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                    std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                     std::pow(std::pow(C47, 7. / 3.) *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                              2))) -
               C164 * C47 *
                   (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                    C81 *
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                        0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                        std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                   C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                   (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                    std::pow(std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                             2))) *
                  C168 +
              ((C156 * C47 *
                    (C81 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) /
                         (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81) +
                     (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81 *
                          (C57 * C81 * 0.0000006608198229 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) /
                               (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81) +
                           C81 * -0.00081290825 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81))) -
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) *
                          std::pow(C47, 7. / 6.) * C81 / std::sqrt(C57)) /
                         std::pow(2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81, 2)) /
                    (std::pow(C47, 7. / 3.) *
                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.)) -
                (C159 * C47 *
                     (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                          C81 +
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) /
                          (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                     std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                 C158 * C47 *
                     (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                          C81 +
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) /
                          (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                     std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1)) /
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                     std::pow(std::pow(C47, 7. / 3.) *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                              2))) +
               C164 * C47 *
                   ((std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                    (-0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) /
                         (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81) +
                     C81 *
                         (std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::pow(C47, 7. / 6.) / (2 * std::sqrt(C57)) +
                          0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) /
                              (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                        C57) /
                   (std::pow(C47, 7. / 3.) *
                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                  C166) +
             ((C156 * C47 *
                   ((std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                    (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81 * C57 *
                         (C81 * -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                              std::pow(std::pow(C47, 7. / 6.) * C81, 2) -
                          0.00081290825 * std::exp(-0.00081290825 * std::sqrt(C57) /
                                                   (std::pow(C47, 7. / 6.) * C81))) -
                     C57 * C81 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         2 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) /
                        std::pow(2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81, 2)) /
                   (std::pow(C47, 7. / 3.) *
                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.)) -
               (C159 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                    C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                C158 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                    C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1)) /
                   (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                    std::pow(std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                             2))) +
              C164 * C47 *
                  (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                       0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                       std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                   (std::pow(std::pow(C47, 7. / 6.) * C81, 2) *
                        (C81 * 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                             std::exp(-0.00081290825 * std::sqrt(C57) /
                                      (std::pow(C47, 7. / 6.) * C81)) *
                             0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                             std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                             0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) -
                    C81 *
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                        0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * 2 * C81 *
                        std::pow(C47, 14. / 6.)) /
                       std::pow(std::pow(C47, 7. / 6.) * C81, 4)) *
                  C57 /
                  (std::pow(C47, 7. / 3.) *
                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                 C174) +
            (((C158 * C47 *
                   std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                   C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * 4. *
                   (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                        std::pow(C47, 7. / 3.) *
                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                        std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                    std::pow(std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                             2) *
                        C52 * std::pow(C60, C52 - 1)) /
                   (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                    std::pow(
                        4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2),
                        2)) +
               C47 *
                   ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                        (C51 - 1) * std::pow(C60, C51 - 2) / (2. * (std::pow(2., C49) - 1.)) -
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                         std::pow(std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  2) *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                         std::pow(C60, C52 - 2) -
                     std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                         C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                         (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                              std::pow(C47, 7. / 3.) *
                              std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                              std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                          std::pow(std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2) *
                              C52 * std::pow(C60, C52 - 1)) /
                         (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                        std::pow(
                            4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(
                                    std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2),
                            2)) *
                   C159) -
              C156 * C47 *
                  (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                       C81 +
                   C57 * C81 * -0.00081290825 *
                       std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) /
                       (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                  std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                  (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                   std::pow(std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                            2))) -
             C164 * C47 *
                 (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                  C81 * std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                      0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                      std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                 C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                 (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                  std::pow(std::pow(C47, 7. / 3.) *
                               std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                           2))) *
                C169;

        matrix.upper[14] =
            (((((C47 *
                     ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                          (C51 - 1) * std::pow(C59, C51 - 2) / (2. * (std::pow(2., C49) - 1.)) -
                      (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                           std::pow(std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2) *
                           std::exp(-0.00081290825 * std::sqrt(C57) /
                                    (std::pow(C47, 7. / 6.) * C81)) *
                           C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                           std::pow(C59, C52 - 2) -
                       std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                           C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * 4. *
                           (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                                std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2) *
                                C52 * std::pow(C59, C52 - 1)) /
                           (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                          std::pow(
                              4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                  std::pow(
                                      std::pow(C47, 7. / 3.) *
                                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                      2),
                              2)) *
                     C168 +
                 C169 * C47 *
                     std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                     C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                     (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                          std::pow(C47, 7. / 3.) *
                          std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                          std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) +
                      std::pow(std::pow(C47, 7. / 3.) *
                                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                               2) *
                          C52 * std::pow(C59, C52 - 1)) /
                     (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                      std::pow(4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                   std::pow(std::pow(C47, 7. / 3.) *
                                                std::sqrt(
                                                    (std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                            2),
                               2))) -
                C166 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                         C81 +
                     C57 * C81 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) /
                         (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                    std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                     std::pow(std::pow(C47, 7. / 3.) *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                              2))) -
               C174 * C47 *
                   (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                    C81 *
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                        0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                        std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                   C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) /
                   (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                    std::pow(std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                             2))) *
                  C168 +
              ((C166 * C47 *
                    (C81 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) /
                         (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81) +
                     (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81 *
                          (C57 * C81 * 0.0000006608198229 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81)) /
                               (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81) +
                           C81 * -0.00081290825 *
                               std::exp(-0.00081290825 * std::sqrt(C57) /
                                        (std::pow(C47, 7. / 6.) * C81))) -
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) *
                          std::pow(C47, 7. / 6.) * C81 / std::sqrt(C57)) /
                         std::pow(2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81, 2)) /
                    (std::pow(C47, 7. / 3.) *
                     std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.)) -
                (C169 * C47 *
                     (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                          C81 +
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) /
                          (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                     std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                 C168 * C47 *
                     (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                          C81 +
                      C57 * C81 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) /
                          (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                     std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1)) /
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                     std::pow(std::pow(C47, 7. / 3.) *
                                  std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                              2))) +
               C174 * C47 *
                   ((std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                    (-0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) /
                         (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81) +
                     C81 *
                         (std::exp(-0.00081290825 * std::sqrt(C57) /
                                   (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::pow(C47, 7. / 6.) / (2 * std::sqrt(C57)) +
                          0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) /
                              (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                        C57) /
                   (std::pow(C47, 7. / 3.) *
                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                  C166) +
             ((C166 * C47 *
                   ((std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) +
                    (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81 * C57 *
                         (C81 * -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C57) /
                                       (std::pow(C47, 7. / 6.) * C81)) *
                              0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                              std::pow(std::pow(C47, 7. / 6.) * C81, 2) -
                          0.00081290825 * std::exp(-0.00081290825 * std::sqrt(C57) /
                                                   (std::pow(C47, 7. / 6.) * C81))) -
                     C57 * C81 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         2 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) /
                        std::pow(2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81, 2)) /
                   (std::pow(C47, 7. / 3.) *
                    std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.)) -
               (C169 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                    C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                C168 * C47 *
                    (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                     C81 *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                         std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                    C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1)) /
                   (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                    std::pow(std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                             2))) +
              C174 * C47 *
                  (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                       0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                       std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                   (std::pow(std::pow(C47, 7. / 6.) * C81, 2) *
                        (C81 * 0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) *
                             std::exp(-0.00081290825 * std::sqrt(C57) /
                                      (std::pow(C47, 7. / 6.) * C81)) *
                             0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                             std::pow(std::pow(C47, 7. / 6.) * C81, 2) +
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                             0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.)) -
                    C81 *
                        std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                        0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * 2 * C81 *
                        std::pow(C47, 14. / 6.)) /
                       std::pow(std::pow(C47, 7. / 6.) * C81, 4)) *
                  C57 /
                  (std::pow(C47, 7. / 3.) *
                   std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) *
                 C174) +
            (((C168 * C47 *
                   std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                   C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C59, C52 - 1) * 4. *
                   (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                        std::pow(C47, 7. / 3.) *
                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                        std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                    std::pow(std::pow(C47, 7. / 3.) *
                                 std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                             2) *
                        C52 * std::pow(C60, C52 - 1)) /
                   (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                    std::pow(
                        4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                            std::pow(std::pow(C47, 7. / 3.) *
                                         std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                     2),
                        2)) +
               C47 *
                   ((C79 - 0.0843 / ((1.3981 * std::sqrt(C63) + 0.2611 * C63) + 1.)) * C51 *
                        (C51 - 1) * std::pow(C60, C51 - 2) / (2. * (std::pow(2., C49) - 1.)) -
                    (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                         std::pow(std::pow(C47, 7. / 3.) *
                                      std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                  2) *
                         std::exp(-0.00081290825 * std::sqrt(C57) /
                                  (std::pow(C47, 7. / 6.) * C81)) *
                         C81 * C57 * std::pow(C47, 7. / 3.) * C52 * (C52 - 1) *
                         std::pow(C60, C52 - 2) -
                     std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                         C81 * C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) * 4. *
                         (std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                              std::pow(C47, 7. / 3.) *
                              std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) * 2 *
                              std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) +
                          std::pow(std::pow(C47, 7. / 3.) *
                                       std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                   2) *
                              C52 * std::pow(C60, C52 - 1)) /
                         (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.))) /
                        std::pow(
                            4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                                std::pow(
                                    std::pow(C47, 7. / 3.) *
                                        std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                                    2),
                            2)) *
                   C169) -
              C166 * C47 *
                  (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                       C81 +
                   C57 * C81 * -0.00081290825 *
                       std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) /
                       (2 * std::sqrt(C57) * std::pow(C47, 7. / 6.) * C81)) *
                  std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                  (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                   std::pow(std::pow(C47, 7. / 3.) *
                                std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                            2))) -
             C174 * C47 *
                 (std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) +
                  C81 * std::exp(-0.00081290825 * std::sqrt(C57) / (std::pow(C47, 7. / 6.) * C81)) *
                      0.00081290825 * std::sqrt(C57) * std::pow(C47, 7. / 6.) /
                      std::pow(std::pow(C47, 7. / 6.) * C81, 2)) *
                 C57 * std::pow(C47, 7. / 3.) * C52 * std::pow(C60, C52 - 1) /
                 (4. * std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.) *
                  std::pow(std::pow(C47, 7. / 3.) *
                               std::sqrt((std::pow(C59, C52) + std::pow(C60, C52)) / 2.),
                           2))) *
                C169;

    } else
    {
        const double C679 = 2. * sigmaAb;
        const double C680 = rhoA + rhoB;
        const double C681 = rhoA - rhoB;
        const double C682 = 1. / 3.;
        const double C683 = 2. / 3.;
        const double C684 = 4. / 3.;
        const double C685 = 5. / 3.;
        const double C686 = 0.9999999999999999 * C681;
        const double C687 = Pi * C680;
        const double C688 = sigmaAa + C679;
        const double C689 = 4. * C687;
        const double C690 = C688 + sigmaBb;
        const double C691 = C686 / C680;
        const double C692 = C691 + 1.;
        const double C693 = 1. - C691;
        const double C694 = 0.22167 / C689;
        const double C695 = 3. / C689;
        const double C696 = std::pow(C695, C682);
        const double C697 = std::pow(C695, C683);
        const double C698 = 7.389e-6 * C697;
        const double C699 = 0.0116 * C696;
        const double C700 = 0.023266 * C696;
        const double C701 = 0.472 * C697;
        const double C702 = 8.723 * C696;
        const double C703 = std::log(C696);
        const double C704 = 0.0311 * C703;
        const double C705 = C696 * C703;
        const double C706 = C700 + C698;
        const double C707 = C702 + C701;
        const double C708 = 0.0020 * C705;
        const double C709 = C706 + 0.002568;
        const double C710 = C707 + C694;
        const double C711 = C704 + C708;
        const double C712 = C710 + 1.;
        const double C713 = C711 - 0.048;
        const double C714 = C709 / C712;
        const double C715 = C714 + 0.001667;
        const double C716 = C713 - C699;
        const double C718 = 1;
        const double C719 = 1;
        const double C720 = 0.9999999999999999 * C719 + 0.;
        const double C721 = Pi * C718;
        const double C722 = 4. * C721;
        const double C723 = C720 / C680 - C718 * C686 / std::pow(C680, 2);
        const double C724 = C723;
        const double C725 = -C723;
        const double C726 = -0.22167 * C722 / std::pow(C689, 2);
        const double C727 = -3. * C722 / std::pow(C689, 2);
        const double C728 = C682 * std::pow(C695, C682 - 1) * C727;
        const double C729 = C683 * std::pow(C695, C683 - 1) * C727;
        const double C730 = 0.7389e-5 * C729 + 0.;
        const double C731 = 0.0116 * C728;
        const double C732 = 0.023266 * C728;
        const double C733 = 0.472 * C729;
        const double C734 = 8.723 * C728;
        const double C735 = C728 / C696;
        const double C736 = 0.0311 * C735;
        const double C737 = C703 * C728 + C696 * C735;
        const double C738 = C732 + C730;
        const double C739 = C734 + C733;
        const double C740 = 0.002 * C737;
        const double C741 = C738;
        const double C742 = C739 + C726;
        const double C743 = C736 + C740;
        const double C744 = C742;
        const double C745 = C743;
        const double C746 = C741 / C712 - C744 * C709 / std::pow(C712, 2);
        const double C747 = C746;
        const double C748 = C745 - C731;
        const double C813 = 0.;
        const double C814 =
            (C813 / C680 +
             ((-C720) / std::pow(C680, 2) - (-C718 * C686 * 2 * C680) / std::pow(C680, 4)) * C718) -
            C720 * C718 / std::pow(C680, 2);
        const double C815 = C814;
        const double C816 = -C814;
        const double C817 = (-C722 * -0.22167 * C722 * 2 * C689) / std::pow(C689, 4);
        const double C818 = (-C722 * -3. * C722 * 2 * C689) / std::pow(C689, 4);
        const double C819 = C682 * (C682 - 1) * std::pow(C695, C682 - 2) * std::pow(C727, 2) +
                            C682 * std::pow(C695, C682 - 1) * C818;
        const double C820 = C683 * (C683 - 1) * std::pow(C695, C683 - 2) * std::pow(C727, 2) +
                            C683 * std::pow(C695, C683 - 1) * C818;
        const double C821 = 0.7389e-5 * C820 + 0.;
        const double C822 = 0.0116 * C819;
        const double C823 = 0.023266 * C819;
        const double C824 = 0.472 * C820;
        const double C825 = 8.723 * C819;
        const double C826 = C819 / C696 - std::pow(C728, 2) / std::pow(C696, 2);
        const double C827 = 0.0311 * C826;
        const double C828 = ((C728 * C735 + C703 * C819) + C735 * C728) + C696 * C826;
        const double C829 = C823 + C821;
        const double C830 = C825 + C824;
        const double C831 = 0.002 * C828;
        const double C832 = C829;
        const double C833 = C830 + C817;
        const double C834 = C827 + C831;
        const double C835 = C833;
        const double C836 = C834;
        const double C837 = ((C832 / C712 + ((-C741) / std::pow(C712, 2) -
                                             (-C744 * C709 * 2 * C712) / std::pow(C712, 4)) *
                                                C744) -
                             C835 * C709 / std::pow(C712, 2)) -
                            C741 * C744 / std::pow(C712, 2);
        const double C838 = C837;
        const double C839 = C836 - C822;
        matrix.upper[0] =
            ((((((((((((((((((((((std::pow(
                                      std::pow(C680, 7. / 3.) *
                                          std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                    2.),
                                      2) *
                                      (C680 *
                                           ((6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2) *
                                                 (std::pow(C680, 7. / 3.) *
                                                      std::sqrt((std::pow(C692, C685) +
                                                                 std::pow(C693, C685)) /
                                                                2.) *
                                                      C690 * C715 *
                                                      (std::exp(-0.00081290825 * std::sqrt(C690) /
                                                                (std::pow(C680, 7. / 6.) * C715)) *
                                                           0.00081290825 * std::sqrt(C690) * C715 *
                                                           7. * std::pow(C680, -5. / 6.) / 6. +
                                                       0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                                           std::pow(C680, 1. / 6.) *
                                                           std::exp(
                                                               -0.00081290825 * std::sqrt(C690) /
                                                               (std::pow(C680, 7. / 6.) * C715)) *
                                                           0.00081290825 * std::sqrt(C690) * C715 *
                                                           7. * std::pow(C680, 1. / 6.) /
                                                           (6. *
                                                            std::pow(std::pow(C680, 7. / 6.) * C715,
                                                                     2))) +
                                                  C690 * C715 *
                                                      std::exp(-0.00081290825 * std::sqrt(C690) /
                                                               (std::pow(C680, 7. / 6.) * C715)) *
                                                      0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                                      std::pow(C680, 1. / 6.) *
                                                      std::sqrt((std::pow(C692, C685) +
                                                                 std::pow(C693, C685)) /
                                                                2.) *
                                                      7. * std::pow(C680, 4. / 3.) / 3.) -
                                             std::pow(C680, 7. / 3.) *
                                                 std::sqrt(
                                                     (std::pow(C692, C685) + std::pow(C693, C685)) /
                                                     2.) *
                                                 C690 * C715 *
                                                 std::exp(-0.00081290825 * std::sqrt(C690) /
                                                          (std::pow(C680, 7. / 6.) * C715)) *
                                                 0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                                 std::pow(C680, 1. / 6.) * 6. *
                                                 std::pow(C680, 7. / 6.) * C715 * 2 * C715 * 7. *
                                                 std::pow(C680, 1. / 6.) / 6.) /
                                                std::pow(
                                                    6. *
                                                        std::pow(std::pow(C680, 7. / 6.) * C715, 2),
                                                    2) -
                                            (std::exp(-0.00081290825 * std::sqrt(C690) /
                                                      (std::pow(C680, 7. / 6.) * C715)) *
                                                 C715 * C690 *
                                                 std::sqrt(
                                                     (std::pow(C692, C685) + std::pow(C693, C685)) /
                                                     2.) *
                                                 28. * std::pow(C680, 1. / 3.) / 3. +
                                             std::sqrt(
                                                 (std::pow(C692, C685) + std::pow(C693, C685)) /
                                                 2.) *
                                                 7. * std::pow(C680, 4. / 3.) * C690 * C715 *
                                                 std::exp(-0.00081290825 * std::sqrt(C690) /
                                                          (std::pow(C680, 7. / 6.) * C715)) *
                                                 0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                                 std::pow(C680, 1. / 6.) /
                                                 (6. *
                                                  std::pow(std::pow(C680, 7. / 6.) * C715, 2))) /
                                                3.) +
                                       (std::pow(C680, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                            C690 * C715 *
                                            std::exp(-0.00081290825 * std::sqrt(C690) /
                                                     (std::pow(C680, 7. / 6.) * C715)) *
                                            0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                            std::pow(C680, 1. / 6.) /
                                            (6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2)) -
                                        std::exp(-0.00081290825 * std::sqrt(C690) /
                                                 (std::pow(C680, 7. / 6.) * C715)) *
                                            C715 * C690 *
                                            std::sqrt(
                                                (std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                            7. * std::pow(C680, 4. / 3.) / 3.)) -
                                  C680 *
                                      (std::pow(C680, 7. / 3.) *
                                           std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                     2.) *
                                           C690 * C715 *
                                           std::exp(-0.00081290825 * std::sqrt(C690) /
                                                    (std::pow(C680, 7. / 6.) * C715)) *
                                           0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                           std::pow(C680, 1. / 6.) /
                                           (6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2)) -
                                       std::exp(-0.00081290825 * std::sqrt(C690) /
                                                (std::pow(C680, 7. / 6.) * C715)) *
                                           C715 * C690 *
                                           std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                     2.) *
                                           7. * std::pow(C680, 4. / 3.) / 3.) *
                                      std::pow(C680, 7. / 3.) *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                      2 *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                      7. * std::pow(C680, 4. / 3.) / 3.) /
                                     std::pow(std::pow(C680, 7. / 3.) *
                                                  std::sqrt((std::pow(C692, C685) +
                                                             std::pow(C693, C685)) /
                                                            2.),
                                              4) +
                                 (std::pow(C680, 7. / 3.) *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                      C690 * C715 *
                                      std::exp(-0.00081290825 * std::sqrt(C690) /
                                               (std::pow(C680, 7. / 6.) * C715)) *
                                      0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                      std::pow(C680, 1. / 6.) /
                                      (6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2)) -
                                  std::exp(-0.00081290825 * std::sqrt(C690) /
                                           (std::pow(C680, 7. / 6.) * C715)) *
                                      C715 * C690 *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                      7. * std::pow(C680, 4. / 3.) / 3.) /
                                     std::pow(std::pow(C680, 7. / 3.) *
                                                  std::sqrt((std::pow(C692, C685) +
                                                             std::pow(C693, C685)) /
                                                            2.),
                                              2)) *
                                    C718 +
                                ((2. - (std::pow(C692, C684) + std::pow(C693, C684))) /
                                     (2. * (std::pow(2., C682) - 1.)) +
                                 1) *
                                    C748) +
                               C735 * 0.01555 *
                                   ((std::pow(C692, C684) + std::pow(C693, C684)) - 2.) /
                                   (2. * (std::pow(2., C682) - 1.))) +
                              C737 * 0.0007 * ((std::pow(C692, C684) + std::pow(C693, C684)) - 2.) /
                                  (2. * (std::pow(2., C682) - 1.))) +
                             C728 * -0.0048 * ((std::pow(C692, C684) + std::pow(C693, C684)) - 2.) /
                                 (2. * (std::pow(2., C682) - 1.))) +
                            ((((((0.01555 * C703 + 0.0007 * C705) - 0.0269) - 0.0048 * C696) -
                               C716) *
                                  C684 * std::pow(C692, C684 - 1) /
                                  (2. * (std::pow(2., C682) - 1.)) -
                              std::exp(-0.00081290825 * std::sqrt(C690) /
                                       (std::pow(C680, 7. / 6.) * C715)) *
                                  C715 * C690 * std::pow(C680, 7. / 3.) * C685 *
                                  std::pow(C692, C685 - 1) /
                                  (4. *
                                   std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                   std::pow(
                                       std::pow(C680, 7. / 3.) *
                                           std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                     2.),
                                       2))) -
                             C680 *
                                 (4. *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                      std::pow(std::pow(C680, 7. / 3.) *
                                                   std::sqrt((std::pow(C692, C685) +
                                                              std::pow(C693, C685)) /
                                                             2.),
                                               2) *
                                      (std::exp(-0.00081290825 * std::sqrt(C690) /
                                                (std::pow(C680, 7. / 6.) * C715)) *
                                           C715 * C690 * C685 * std::pow(C692, C685 - 1) * 7. *
                                           std::pow(C680, 4. / 3.) / 3. +
                                       std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) *
                                           C690 * C715 *
                                           std::exp(-0.00081290825 * std::sqrt(C690) /
                                                    (std::pow(C680, 7. / 6.) * C715)) *
                                           0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                           std::pow(C680, 1. / 6.) /
                                           (6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2))) -
                                  std::exp(-0.00081290825 * std::sqrt(C690) /
                                           (std::pow(C680, 7. / 6.) * C715)) *
                                      C715 * C690 * std::pow(C680, 7. / 3.) * C685 *
                                      std::pow(C692, C685 - 1) * 4. *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                      std::pow(C680, 7. / 3.) *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                      2 *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                      7. * std::pow(C680, 4. / 3.) / 3.) /
                                 std::pow(
                                     4. *
                                         std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                   2.) *
                                         std::pow(std::pow(C680, 7. / 3.) *
                                                      std::sqrt((std::pow(C692, C685) +
                                                                 std::pow(C693, C685)) /
                                                                2.),
                                                  2),
                                     2)) *
                                C724) +
                           ((((((0.01555 * C703 + 0.0007 * C705) - 0.0269) - 0.0048 * C696) -
                              C716) *
                                 C684 * std::pow(C693, C684 - 1) /
                                 (2. * (std::pow(2., C682) - 1.)) -
                             std::exp(-0.00081290825 * std::sqrt(C690) /
                                      (std::pow(C680, 7. / 6.) * C715)) *
                                 C715 * C690 * std::pow(C680, 7. / 3.) * C685 *
                                 std::pow(C693, C685 - 1) /
                                 (4. *
                                  std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                  std::pow(
                                      std::pow(C680, 7. / 3.) *
                                          std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                    2.),
                                      2))) -
                            C680 *
                                (4. *
                                     std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                     std::pow(std::pow(C680, 7. / 3.) *
                                                  std::sqrt((std::pow(C692, C685) +
                                                             std::pow(C693, C685)) /
                                                            2.),
                                              2) *
                                     (std::exp(-0.00081290825 * std::sqrt(C690) /
                                               (std::pow(C680, 7. / 6.) * C715)) *
                                          C715 * C690 * C685 * std::pow(C693, C685 - 1) * 7. *
                                          std::pow(C680, 4. / 3.) / 3. +
                                      std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) *
                                          C690 * C715 *
                                          std::exp(-0.00081290825 * std::sqrt(C690) /
                                                   (std::pow(C680, 7. / 6.) * C715)) *
                                          0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                          std::pow(C680, 1. / 6.) /
                                          (6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2))) -
                                 std::exp(-0.00081290825 * std::sqrt(C690) /
                                          (std::pow(C680, 7. / 6.) * C715)) *
                                     C715 * C690 * std::pow(C680, 7. / 3.) * C685 *
                                     std::pow(C693, C685 - 1) * 4. *
                                     std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                     std::pow(C680, 7. / 3.) *
                                     std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                     2 *
                                     std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                     7. * std::pow(C680, 4. / 3.) / 3.) /
                                std::pow(
                                    4. *
                                        std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                  2.) *
                                        std::pow(std::pow(C680, 7. / 3.) *
                                                     std::sqrt((std::pow(C692, C685) +
                                                                std::pow(C693, C685)) /
                                                               2.),
                                                 2),
                                    2)) *
                               C725) +
                          (std::pow(C680, 7. / 3.) *
                               std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                               C747 *
                               (C680 *
                                    (std::exp(-0.00081290825 * std::sqrt(C690) /
                                              (std::pow(C680, 7. / 6.) * C715)) *
                                         0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                         std::pow(C680, 1. / 6.) /
                                         (6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2)) +
                                     (std::pow(std::pow(C680, 7. / 6.) * C715, 2) * C715 *
                                          (std::exp(-0.00081290825 * std::sqrt(C690) /
                                                    (std::pow(C680, 7. / 6.) * C715)) *
                                               0.00081290825 * std::sqrt(C690) * 7. *
                                               std::pow(C680, 1. / 6.) / 6. +
                                           0.00081290825 * std::sqrt(C690) *
                                               std::pow(C680, 7. / 6.) *
                                               std::exp(-0.00081290825 * std::sqrt(C690) /
                                                        (std::pow(C680, 7. / 6.) * C715)) *
                                               0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                               std::pow(C680, 1. / 6.) /
                                               (6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2))) -
                                      C715 *
                                          std::exp(-0.00081290825 * std::sqrt(C690) /
                                                   (std::pow(C680, 7. / 6.) * C715)) *
                                          0.00081290825 * std::sqrt(C690) *
                                          std::pow(C680, 7. / 6.) * std::pow(C680, 7. / 6.) * C715 *
                                          2 * C715 * 7. * std::pow(C680, 1. / 6.) / 6.) /
                                         std::pow(std::pow(C680, 7. / 6.) * C715, 4)) *
                                    C690 +
                                (std::exp(-0.00081290825 * std::sqrt(C690) /
                                          (std::pow(C680, 7. / 6.) * C715)) +
                                 C715 *
                                     std::exp(-0.00081290825 * std::sqrt(C690) /
                                              (std::pow(C680, 7. / 6.) * C715)) *
                                     0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                                     std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                                    C690) -
                           C747 * C680 *
                               (std::exp(-0.00081290825 * std::sqrt(C690) /
                                         (std::pow(C680, 7. / 6.) * C715)) +
                                C715 *
                                    std::exp(-0.00081290825 * std::sqrt(C690) /
                                             (std::pow(C680, 7. / 6.) * C715)) *
                                    0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                                    std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                               C690 *
                               std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) * 7. *
                               std::pow(C680, 4. / 3.) / 3.) /
                              std::pow(
                                  std::pow(C680, 7. / 3.) *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                  2)) *
                             C718 +
                         (((((((((std::pow(
                                      std::pow(C680, 7. / 3.) *
                                          std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                    2.),
                                      2) *
                                      C680 *
                                      (C690 * C715 *
                                           std::exp(-0.00081290825 * std::sqrt(C690) /
                                                    (std::pow(C680, 7. / 6.) * C715)) *
                                           0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                           std::pow(C680, 1. / 6.) * std::pow(C680, 7. / 3.) *
                                           C685 * std::pow(C692, C685 - 1) /
                                           (4. *
                                            std::sqrt(
                                                (std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                            6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2)) -
                                       std::exp(-0.00081290825 * std::sqrt(C690) /
                                                (std::pow(C680, 7. / 6.) * C715)) *
                                           C715 * C690 * 7. * std::pow(C680, 4. / 3.) * C685 *
                                           std::pow(C692, C685 - 1) /
                                           (12. * std::sqrt((std::pow(C692, C685) +
                                                             std::pow(C693, C685)) /
                                                            2.))) -
                                  C680 *
                                      (std::pow(C680, 7. / 3.) *
                                           std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                     2.) *
                                           C690 * C715 *
                                           std::exp(-0.00081290825 * std::sqrt(C690) /
                                                    (std::pow(C680, 7. / 6.) * C715)) *
                                           0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                           std::pow(C680, 1. / 6.) /
                                           (6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2)) -
                                       std::exp(-0.00081290825 * std::sqrt(C690) /
                                                (std::pow(C680, 7. / 6.) * C715)) *
                                           C715 * C690 *
                                           std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                     2.) *
                                           7. * std::pow(C680, 4. / 3.) / 3.) *
                                      std::pow(C680, 7. / 3.) *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                      2 * std::pow(C680, 7. / 3.) * C685 *
                                      std::pow(C692, C685 - 1) /
                                      (4. *
                                       std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                 2.))) /
                                     std::pow(std::pow(C680, 7. / 3.) *
                                                  std::sqrt((std::pow(C692, C685) +
                                                             std::pow(C693, C685)) /
                                                            2.),
                                              4) +
                                 (((((0.01555 * C703 + 0.0007 * C705) - 0.0269) - 0.0048 * C696) -
                                   C716) *
                                      C684 * std::pow(C692, C684 - 1) /
                                      (2. * (std::pow(2., C682) - 1.)) -
                                  std::exp(-0.00081290825 * std::sqrt(C690) /
                                           (std::pow(C680, 7. / 6.) * C715)) *
                                      C715 * C690 * std::pow(C680, 7. / 3.) * C685 *
                                      std::pow(C692, C685 - 1) /
                                      (4. *
                                       std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                 2.) *
                                       std::pow(std::pow(C680, 7. / 3.) *
                                                    std::sqrt((std::pow(C692, C685) +
                                                               std::pow(C693, C685)) /
                                                              2.),
                                                2)))) *
                                    C718 -
                                C748 * C680 * C684 * std::pow(C692, C684 - 1) /
                                    (2. * (std::pow(2., C682) - 1.))) +
                               C735 * C680 * 0.01555 * C684 * std::pow(C692, C684 - 1) /
                                   (2. * (std::pow(2., C682) - 1.))) +
                              C737 * C680 * 0.0007 * C684 * std::pow(C692, C684 - 1) /
                                  (2. * (std::pow(2., C682) - 1.))) +
                             C728 * C680 * -0.0048 * C684 * std::pow(C692, C684 - 1) /
                                 (2. * (std::pow(2., C682) - 1.))) +
                            C680 *
                                (((((0.01555 * C703 + 0.0007 * C705) - 0.0269) - 0.0048 * C696) -
                                  C716) *
                                     C684 * (C684 - 1) * std::pow(C692, C684 - 2) /
                                     (2. * (std::pow(2., C682) - 1.)) -
                                 (4. *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                      std::pow(std::pow(C680, 7. / 3.) *
                                                   std::sqrt((std::pow(C692, C685) +
                                                              std::pow(C693, C685)) /
                                                             2.),
                                               2) *
                                      std::exp(-0.00081290825 * std::sqrt(C690) /
                                               (std::pow(C680, 7. / 6.) * C715)) *
                                      C715 * C690 * std::pow(C680, 7. / 3.) * C685 * (C685 - 1) *
                                      std::pow(C692, C685 - 2) -
                                  std::exp(-0.00081290825 * std::sqrt(C690) /
                                           (std::pow(C680, 7. / 6.) * C715)) *
                                      C715 * C690 * std::pow(C680, 7. / 3.) * C685 *
                                      std::pow(C692, C685 - 1) * 4. *
                                      (std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                 2.) *
                                           std::pow(C680, 7. / 3.) *
                                           std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                     2.) *
                                           2 * std::pow(C680, 7. / 3.) * C685 *
                                           std::pow(C692, C685 - 1) +
                                       std::pow(std::pow(C680, 7. / 3.) *
                                                    std::sqrt((std::pow(C692, C685) +
                                                               std::pow(C693, C685)) /
                                                              2.),
                                                2) *
                                           C685 * std::pow(C692, C685 - 1)) /
                                      (4. *
                                       std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                 2.))) /
                                     std::pow(4. *
                                                  std::sqrt((std::pow(C692, C685) +
                                                             std::pow(C693, C685)) /
                                                            2.) *
                                                  std::pow(std::pow(C680, 7. / 3.) *
                                                               std::sqrt((std::pow(C692, C685) +
                                                                          std::pow(C693, C685)) /
                                                                         2.),
                                                           2),
                                              2)) *
                                C724) +
                           C725 * C680 *
                               std::exp(-0.00081290825 * std::sqrt(C690) /
                                        (std::pow(C680, 7. / 6.) * C715)) *
                               C715 * C690 * std::pow(C680, 7. / 3.) * C685 *
                               std::pow(C693, C685 - 1) * 4. *
                               (std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                    std::pow(C680, 7. / 3.) *
                                    std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                    2 * std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) +
                                std::pow(
                                    std::pow(C680, 7. / 3.) *
                                        std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                  2.),
                                    2) *
                                    C685 * std::pow(C692, C685 - 1)) /
                               (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                std::pow(
                                    4. *
                                        std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                  2.) *
                                        std::pow(std::pow(C680, 7. / 3.) *
                                                     std::sqrt((std::pow(C692, C685) +
                                                                std::pow(C693, C685)) /
                                                               2.),
                                                 2),
                                    2))) -
                          C747 * C680 *
                              (std::exp(-0.00081290825 * std::sqrt(C690) /
                                        (std::pow(C680, 7. / 6.) * C715)) +
                               C715 *
                                   std::exp(-0.00081290825 * std::sqrt(C690) /
                                            (std::pow(C680, 7. / 6.) * C715)) *
                                   0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                                   std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                              C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) /
                              (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                               std::pow(std::pow(C680, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                        2))) *
                             C724) +
                        (((((((((std::pow(
                                     std::pow(C680, 7. / 3.) *
                                         std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                   2.),
                                     2) *
                                     C680 *
                                     (C690 * C715 *
                                          std::exp(-0.00081290825 * std::sqrt(C690) /
                                                   (std::pow(C680, 7. / 6.) * C715)) *
                                          0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                          std::pow(C680, 1. / 6.) * std::pow(C680, 7. / 3.) * C685 *
                                          std::pow(C693, C685 - 1) /
                                          (4. *
                                           std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                     2.) *
                                           6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2)) -
                                      std::exp(-0.00081290825 * std::sqrt(C690) /
                                               (std::pow(C680, 7. / 6.) * C715)) *
                                          C715 * C690 * 7. * std::pow(C680, 4. / 3.) * C685 *
                                          std::pow(C693, C685 - 1) /
                                          (12. *
                                           std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                     2.))) -
                                 C680 *
                                     (std::pow(C680, 7. / 3.) *
                                          std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                    2.) *
                                          C690 * C715 *
                                          std::exp(-0.00081290825 * std::sqrt(C690) /
                                                   (std::pow(C680, 7. / 6.) * C715)) *
                                          0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                          std::pow(C680, 1. / 6.) /
                                          (6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2)) -
                                      std::exp(-0.00081290825 * std::sqrt(C690) /
                                               (std::pow(C680, 7. / 6.) * C715)) *
                                          C715 * C690 *
                                          std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                    2.) *
                                          7. * std::pow(C680, 4. / 3.) / 3.) *
                                     std::pow(C680, 7. / 3.) *
                                     std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                     2 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) /
                                     (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                     2.))) /
                                    std::pow(
                                        std::pow(C680, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                        4) +
                                (((((0.01555 * C703 + 0.0007 * C705) - 0.0269) - 0.0048 * C696) -
                                  C716) *
                                     C684 * std::pow(C693, C684 - 1) /
                                     (2. * (std::pow(2., C682) - 1.)) -
                                 std::exp(-0.00081290825 * std::sqrt(C690) /
                                          (std::pow(C680, 7. / 6.) * C715)) *
                                     C715 * C690 * std::pow(C680, 7. / 3.) * C685 *
                                     std::pow(C693, C685 - 1) /
                                     (4. *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                      std::pow(std::pow(C680, 7. / 3.) *
                                                   std::sqrt((std::pow(C692, C685) +
                                                              std::pow(C693, C685)) /
                                                             2.),
                                               2)))) *
                                   C718 -
                               C748 * C680 * C684 * std::pow(C693, C684 - 1) /
                                   (2. * (std::pow(2., C682) - 1.))) +
                              C735 * C680 * 0.01555 * C684 * std::pow(C693, C684 - 1) /
                                  (2. * (std::pow(2., C682) - 1.))) +
                             C737 * C680 * 0.0007 * C684 * std::pow(C693, C684 - 1) /
                                 (2. * (std::pow(2., C682) - 1.))) +
                            C728 * C680 * -0.0048 * C684 * std::pow(C693, C684 - 1) /
                                (2. * (std::pow(2., C682) - 1.))) +
                           C724 * C680 *
                               std::exp(-0.00081290825 * std::sqrt(C690) /
                                        (std::pow(C680, 7. / 6.) * C715)) *
                               C715 * C690 * std::pow(C680, 7. / 3.) * C685 *
                               std::pow(C692, C685 - 1) * 4. *
                               (std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                    std::pow(C680, 7. / 3.) *
                                    std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                    2 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) +
                                std::pow(
                                    std::pow(C680, 7. / 3.) *
                                        std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                  2.),
                                    2) *
                                    C685 * std::pow(C693, C685 - 1)) /
                               (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                std::pow(
                                    4. *
                                        std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                  2.) *
                                        std::pow(std::pow(C680, 7. / 3.) *
                                                     std::sqrt((std::pow(C692, C685) +
                                                                std::pow(C693, C685)) /
                                                               2.),
                                                 2),
                                    2))) +
                          C680 *
                              (((((0.01555 * C703 + 0.0007 * C705) - 0.0269) - 0.0048 * C696) -
                                C716) *
                                   C684 * (C684 - 1) * std::pow(C693, C684 - 2) /
                                   (2. * (std::pow(2., C682) - 1.)) -
                               (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                    std::pow(
                                        std::pow(C680, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                        2) *
                                    std::exp(-0.00081290825 * std::sqrt(C690) /
                                             (std::pow(C680, 7. / 6.) * C715)) *
                                    C715 * C690 * std::pow(C680, 7. / 3.) * C685 * (C685 - 1) *
                                    std::pow(C693, C685 - 2) -
                                std::exp(-0.00081290825 * std::sqrt(C690) /
                                         (std::pow(C680, 7. / 6.) * C715)) *
                                    C715 * C690 * std::pow(C680, 7. / 3.) * C685 *
                                    std::pow(C693, C685 - 1) * 4. *
                                    (std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                         std::pow(C680, 7. / 3.) *
                                         std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                   2.) *
                                         2 * std::pow(C680, 7. / 3.) * C685 *
                                         std::pow(C693, C685 - 1) +
                                     std::pow(std::pow(C680, 7. / 3.) *
                                                  std::sqrt((std::pow(C692, C685) +
                                                             std::pow(C693, C685)) /
                                                            2.),
                                              2) *
                                         C685 * std::pow(C693, C685 - 1)) /
                                    (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                    2.))) /
                                   std::pow(
                                       4. *
                                           std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                     2.) *
                                           std::pow(std::pow(C680, 7. / 3.) *
                                                        std::sqrt((std::pow(C692, C685) +
                                                                   std::pow(C693, C685)) /
                                                                  2.),
                                                    2),
                                       2)) *
                              C725) -
                         C747 * C680 *
                             (std::exp(-0.00081290825 * std::sqrt(C690) /
                                       (std::pow(C680, 7. / 6.) * C715)) +
                              C715 *
                                  std::exp(-0.00081290825 * std::sqrt(C690) /
                                           (std::pow(C680, 7. / 6.) * C715)) *
                                  0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                                  std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                             C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) /
                             (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                              std::pow(
                                  std::pow(C680, 7. / 3.) *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                  2))) *
                            C725) +
                       ((((C680 *
                               ((6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2) *
                                     std::pow(C680, 7. / 3.) *
                                     std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                     C690 *
                                     (C715 * (std::exp(-0.00081290825 * std::sqrt(C690) /
                                                       (std::pow(C680, 7. / 6.) * C715)) *
                                                  0.00081290825 * std::sqrt(C690) * 7. *
                                                  std::pow(C680, 1. / 6.) +
                                              0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                                  std::pow(C680, 1. / 6.) *
                                                  std::exp(-0.00081290825 * std::sqrt(C690) /
                                                           (std::pow(C680, 7. / 6.) * C715)) *
                                                  0.00081290825 * std::sqrt(C690) *
                                                  std::pow(C680, 7. / 6.) /
                                                  std::pow(std::pow(C680, 7. / 6.) * C715, 2)) +
                                      std::exp(-0.00081290825 * std::sqrt(C690) /
                                               (std::pow(C680, 7. / 6.) * C715)) *
                                          0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                          std::pow(C680, 1. / 6.)) -
                                 std::pow(C680, 7. / 3.) *
                                     std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                     C690 * C715 *
                                     std::exp(-0.00081290825 * std::sqrt(C690) /
                                              (std::pow(C680, 7. / 6.) * C715)) *
                                     0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                     std::pow(C680, 1. / 6.) * 12. * C715 *
                                     std::pow(C680, 14. / 6.)) /
                                    std::pow(6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2), 2) -
                                (std::exp(-0.00081290825 * std::sqrt(C690) /
                                          (std::pow(C680, 7. / 6.) * C715)) +
                                 C715 *
                                     std::exp(-0.00081290825 * std::sqrt(C690) /
                                              (std::pow(C680, 7. / 6.) * C715)) *
                                     0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                                     std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                                    C690 *
                                    std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                    7. * std::pow(C680, 4. / 3.) / 3.) /
                               std::pow(std::pow(C680, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                        2) +
                           (std::exp(-0.00081290825 * std::sqrt(C690) /
                                     (std::pow(C680, 7. / 6.) * C715)) +
                            C715 *
                                std::exp(-0.00081290825 * std::sqrt(C690) /
                                         (std::pow(C680, 7. / 6.) * C715)) *
                                0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                                std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                               C690 /
                               (std::pow(C680, 7. / 3.) *
                                std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.))) *
                              C718 -
                          C724 * C680 *
                              (std::exp(-0.00081290825 * std::sqrt(C690) /
                                        (std::pow(C680, 7. / 6.) * C715)) +
                               C715 *
                                   std::exp(-0.00081290825 * std::sqrt(C690) /
                                            (std::pow(C680, 7. / 6.) * C715)) *
                                   0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                                   std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                              C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) /
                              (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                               std::pow(std::pow(C680, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                        2))) -
                         C725 * C680 *
                             (std::exp(-0.00081290825 * std::sqrt(C690) /
                                       (std::pow(C680, 7. / 6.) * C715)) +
                              C715 *
                                  std::exp(-0.00081290825 * std::sqrt(C690) /
                                           (std::pow(C680, 7. / 6.) * C715)) *
                                  0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                                  std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                             C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) /
                             (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                              std::pow(
                                  std::pow(C680, 7. / 3.) *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                  2))) +
                        C747 * C680 *
                            (std::exp(-0.00081290825 * std::sqrt(C690) /
                                      (std::pow(C680, 7. / 6.) * C715)) *
                                 0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                                 std::pow(std::pow(C680, 7. / 6.) * C715, 2) +
                             (std::pow(std::pow(C680, 7. / 6.) * C715, 2) *
                                  (C715 * 0.00081290825 * std::sqrt(C690) *
                                       std::pow(C680, 7. / 6.) *
                                       std::exp(-0.00081290825 * std::sqrt(C690) /
                                                (std::pow(C680, 7. / 6.) * C715)) *
                                       0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                                       std::pow(std::pow(C680, 7. / 6.) * C715, 2) +
                                   std::exp(-0.00081290825 * std::sqrt(C690) /
                                            (std::pow(C680, 7. / 6.) * C715)) *
                                       0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.)) -
                              C715 *
                                  std::exp(-0.00081290825 * std::sqrt(C690) /
                                           (std::pow(C680, 7. / 6.) * C715)) *
                                  0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * 2 *
                                  C715 * std::pow(C680, 14. / 6.)) /
                                 std::pow(std::pow(C680, 7. / 6.) * C715, 4)) *
                            C690 /
                            (std::pow(C680, 7. / 3.) *
                             std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.))) *
                           C747) +
                      ((((2. - (std::pow(C692, C684) + std::pow(C693, C684))) /
                             (2. * (std::pow(2., C682) - 1.)) +
                         1) *
                            C718 -
                        C724 * C680 * C684 * std::pow(C692, C684 - 1) /
                            (2. * (std::pow(2., C682) - 1.))) -
                       C725 * C680 * C684 * std::pow(C693, C684 - 1) /
                           (2. * (std::pow(2., C682) - 1.))) *
                          C748) +
                     C735 *
                         ((C718 * 0.01555 * ((std::pow(C692, C684) + std::pow(C693, C684)) - 2.) +
                           C724 * C680 * 0.01555 * C684 * std::pow(C692, C684 - 1)) +
                          C725 * C680 * 0.01555 * C684 * std::pow(C693, C684 - 1)) /
                         (2. * (std::pow(2., C682) - 1.))) +
                    C737 *
                        ((C718 * 0.0007 * ((std::pow(C692, C684) + std::pow(C693, C684)) - 2.) +
                          C724 * C680 * 0.0007 * C684 * std::pow(C692, C684 - 1)) +
                         C725 * C680 * 0.0007 * C684 * std::pow(C693, C684 - 1)) /
                        (2. * (std::pow(2., C682) - 1.))) +
                   C728 *
                       ((C718 * -0.0048 * ((std::pow(C692, C684) + std::pow(C693, C684)) - 2.) +
                         C724 * C680 * -0.0048 * C684 * std::pow(C692, C684 - 1)) +
                        C725 * C680 * -0.0048 * C684 * std::pow(C693, C684 - 1)) /
                       (2. * (std::pow(2., C682) - 1.))) +
                  C680 *
                      ((2. - (std::pow(C692, C684) + std::pow(C693, C684))) /
                           (2. * (std::pow(2., C682) - 1.)) +
                       1) *
                      C839) +
                 C826 * C680 * 0.01555 * ((std::pow(C692, C684) + std::pow(C693, C684)) - 2.) /
                     (2. * (std::pow(2., C682) - 1.))) +
                C828 * C680 * 0.0007 * ((std::pow(C692, C684) + std::pow(C693, C684)) - 2.) /
                    (2. * (std::pow(2., C682) - 1.))) +
               C819 * C680 * -0.0048 * ((std::pow(C692, C684) + std::pow(C693, C684)) - 2.) /
                   (2. * (std::pow(2., C682) - 1.))) +
              C680 *
                  (((((0.01555 * C703 + 0.0007 * C705) - 0.0269) - 0.0048 * C696) - C716) * C684 *
                       std::pow(C692, C684 - 1) / (2. * (std::pow(2., C682) - 1.)) -
                   std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                       C715 * C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) /
                       (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                        std::pow(std::pow(C680, 7. / 3.) *
                                     std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                 2))) *
                  C815) +
             C680 *
                 (((((0.01555 * C703 + 0.0007 * C705) - 0.0269) - 0.0048 * C696) - C716) * C684 *
                      std::pow(C693, C684 - 1) / (2. * (std::pow(2., C682) - 1.)) -
                  std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                      C715 * C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) /
                      (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                       std::pow(std::pow(C680, 7. / 3.) *
                                    std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                2))) *
                 C816) +
            C838 * C680 *
                (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                 C715 *
                     std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                     0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                     std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                C690 /
                (std::pow(C680, 7. / 3.) *
                 std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.));

        const double C749 = 1;
        const double C750 = -1;
        const double C751 = 0.9999999999999999 * C750 + 0.;
        const double C752 = Pi * C749;
        const double C753 = 4. * C752;
        const double C754 = C751 / C680 - C749 * C686 / std::pow(C680, 2);
        const double C755 = C754;
        const double C756 = -C754;
        const double C757 = -0.22167 * C753 / std::pow(C689, 2);
        const double C758 = -3. * C753 / std::pow(C689, 2);
        const double C759 = C682 * std::pow(C695, C682 - 1) * C758;
        const double C760 = C683 * std::pow(C695, C683 - 1) * C758;
        const double C761 = 0.7389e-5 * C760 + 0.;
        const double C762 = 0.0116 * C759;
        const double C763 = 0.023266 * C759;
        const double C764 = 0.472 * C760;
        const double C765 = 8.723 * C759;
        const double C766 = C759 / C696;
        const double C767 = 0.0311 * C766;
        const double C768 = C703 * C759 + C696 * C766;
        const double C769 = C763 + C761;
        const double C770 = C765 + C764;
        const double C771 = 0.002 * C768;
        const double C772 = C769;
        const double C773 = C770 + C757;
        const double C774 = C767 + C771;
        const double C775 = C773;
        const double C776 = C774;
        const double C777 = C772 / C712 - C775 * C709 / std::pow(C712, 2);
        const double C778 = C777;
        const double C779 = C776 - C762;
        const double C840 = 0.;
        const double C841 =
            (C840 / C680 +
             ((-C720) / std::pow(C680, 2) - (-C718 * C686 * 2 * C680) / std::pow(C680, 4)) * C749) -
            C751 * C718 / std::pow(C680, 2);
        const double C842 = C841;
        const double C843 = -C841;
        const double C844 = (-C753 * -0.22167 * C722 * 2 * C689) / std::pow(C689, 4);
        const double C845 = (-C753 * -3. * C722 * 2 * C689) / std::pow(C689, 4);
        const double C846 = C682 * (C682 - 1) * std::pow(C695, C682 - 2) * C727 * C758 +
                            C682 * std::pow(C695, C682 - 1) * C845;
        const double C847 = C683 * (C683 - 1) * std::pow(C695, C683 - 2) * C727 * C758 +
                            C683 * std::pow(C695, C683 - 1) * C845;
        const double C848 = 0.7389e-5 * C847 + 0.;
        const double C849 = 0.0116 * C846;
        const double C850 = 0.023266 * C846;
        const double C851 = 0.472 * C847;
        const double C852 = 8.723 * C846;
        const double C853 = C846 / C696 - C759 * C728 / std::pow(C696, 2);
        const double C854 = 0.0311 * C853;
        const double C855 = ((C728 * C766 + C703 * C846) + C735 * C759) + C696 * C853;
        const double C856 = C850 + C848;
        const double C857 = C852 + C851;
        const double C858 = 0.002 * C855;
        const double C859 = C856;
        const double C860 = C857 + C844;
        const double C861 = C854 + C858;
        const double C862 = C860;
        const double C863 = C861;
        const double C864 = ((C859 / C712 + ((-C741) / std::pow(C712, 2) -
                                             (-C744 * C709 * 2 * C712) / std::pow(C712, 4)) *
                                                C775) -
                             C862 * C709 / std::pow(C712, 2)) -
                            C772 * C744 / std::pow(C712, 2);
        const double C865 = C864;
        const double C866 = C863 - C849;
        matrix.upper[1] =
            ((((((((((((((((((((((std::pow(
                                      std::pow(C680, 7. / 3.) *
                                          std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                    2.),
                                      2) *
                                      (C680 *
                                           ((6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2) *
                                                 (std::pow(C680, 7. / 3.) *
                                                      std::sqrt((std::pow(C692, C685) +
                                                                 std::pow(C693, C685)) /
                                                                2.) *
                                                      C690 * C715 *
                                                      (std::exp(-0.00081290825 * std::sqrt(C690) /
                                                                (std::pow(C680, 7. / 6.) * C715)) *
                                                           0.00081290825 * std::sqrt(C690) * C715 *
                                                           7. * std::pow(C680, -5. / 6.) / 6. +
                                                       0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                                           std::pow(C680, 1. / 6.) *
                                                           std::exp(
                                                               -0.00081290825 * std::sqrt(C690) /
                                                               (std::pow(C680, 7. / 6.) * C715)) *
                                                           0.00081290825 * std::sqrt(C690) * C715 *
                                                           7. * std::pow(C680, 1. / 6.) /
                                                           (6. *
                                                            std::pow(std::pow(C680, 7. / 6.) * C715,
                                                                     2))) +
                                                  C690 * C715 *
                                                      std::exp(-0.00081290825 * std::sqrt(C690) /
                                                               (std::pow(C680, 7. / 6.) * C715)) *
                                                      0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                                      std::pow(C680, 1. / 6.) *
                                                      std::sqrt((std::pow(C692, C685) +
                                                                 std::pow(C693, C685)) /
                                                                2.) *
                                                      7. * std::pow(C680, 4. / 3.) / 3.) -
                                             std::pow(C680, 7. / 3.) *
                                                 std::sqrt(
                                                     (std::pow(C692, C685) + std::pow(C693, C685)) /
                                                     2.) *
                                                 C690 * C715 *
                                                 std::exp(-0.00081290825 * std::sqrt(C690) /
                                                          (std::pow(C680, 7. / 6.) * C715)) *
                                                 0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                                 std::pow(C680, 1. / 6.) * 6. *
                                                 std::pow(C680, 7. / 6.) * C715 * 2 * C715 * 7. *
                                                 std::pow(C680, 1. / 6.) / 6.) /
                                                std::pow(
                                                    6. *
                                                        std::pow(std::pow(C680, 7. / 6.) * C715, 2),
                                                    2) -
                                            (std::exp(-0.00081290825 * std::sqrt(C690) /
                                                      (std::pow(C680, 7. / 6.) * C715)) *
                                                 C715 * C690 *
                                                 std::sqrt(
                                                     (std::pow(C692, C685) + std::pow(C693, C685)) /
                                                     2.) *
                                                 28. * std::pow(C680, 1. / 3.) / 3. +
                                             std::sqrt(
                                                 (std::pow(C692, C685) + std::pow(C693, C685)) /
                                                 2.) *
                                                 7. * std::pow(C680, 4. / 3.) * C690 * C715 *
                                                 std::exp(-0.00081290825 * std::sqrt(C690) /
                                                          (std::pow(C680, 7. / 6.) * C715)) *
                                                 0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                                 std::pow(C680, 1. / 6.) /
                                                 (6. *
                                                  std::pow(std::pow(C680, 7. / 6.) * C715, 2))) /
                                                3.) +
                                       (std::pow(C680, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                            C690 * C715 *
                                            std::exp(-0.00081290825 * std::sqrt(C690) /
                                                     (std::pow(C680, 7. / 6.) * C715)) *
                                            0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                            std::pow(C680, 1. / 6.) /
                                            (6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2)) -
                                        std::exp(-0.00081290825 * std::sqrt(C690) /
                                                 (std::pow(C680, 7. / 6.) * C715)) *
                                            C715 * C690 *
                                            std::sqrt(
                                                (std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                            7. * std::pow(C680, 4. / 3.) / 3.)) -
                                  C680 *
                                      (std::pow(C680, 7. / 3.) *
                                           std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                     2.) *
                                           C690 * C715 *
                                           std::exp(-0.00081290825 * std::sqrt(C690) /
                                                    (std::pow(C680, 7. / 6.) * C715)) *
                                           0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                           std::pow(C680, 1. / 6.) /
                                           (6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2)) -
                                       std::exp(-0.00081290825 * std::sqrt(C690) /
                                                (std::pow(C680, 7. / 6.) * C715)) *
                                           C715 * C690 *
                                           std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                     2.) *
                                           7. * std::pow(C680, 4. / 3.) / 3.) *
                                      std::pow(C680, 7. / 3.) *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                      2 *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                      7. * std::pow(C680, 4. / 3.) / 3.) /
                                     std::pow(std::pow(C680, 7. / 3.) *
                                                  std::sqrt((std::pow(C692, C685) +
                                                             std::pow(C693, C685)) /
                                                            2.),
                                              4) +
                                 (std::pow(C680, 7. / 3.) *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                      C690 * C715 *
                                      std::exp(-0.00081290825 * std::sqrt(C690) /
                                               (std::pow(C680, 7. / 6.) * C715)) *
                                      0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                      std::pow(C680, 1. / 6.) /
                                      (6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2)) -
                                  std::exp(-0.00081290825 * std::sqrt(C690) /
                                           (std::pow(C680, 7. / 6.) * C715)) *
                                      C715 * C690 *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                      7. * std::pow(C680, 4. / 3.) / 3.) /
                                     std::pow(std::pow(C680, 7. / 3.) *
                                                  std::sqrt((std::pow(C692, C685) +
                                                             std::pow(C693, C685)) /
                                                            2.),
                                              2)) *
                                    C718 +
                                ((2. - (std::pow(C692, C684) + std::pow(C693, C684))) /
                                     (2. * (std::pow(2., C682) - 1.)) +
                                 1) *
                                    C748) +
                               C735 * 0.01555 *
                                   ((std::pow(C692, C684) + std::pow(C693, C684)) - 2.) /
                                   (2. * (std::pow(2., C682) - 1.))) +
                              C737 * 0.0007 * ((std::pow(C692, C684) + std::pow(C693, C684)) - 2.) /
                                  (2. * (std::pow(2., C682) - 1.))) +
                             C728 * -0.0048 * ((std::pow(C692, C684) + std::pow(C693, C684)) - 2.) /
                                 (2. * (std::pow(2., C682) - 1.))) +
                            ((((((0.01555 * C703 + 0.0007 * C705) - 0.0269) - 0.0048 * C696) -
                               C716) *
                                  C684 * std::pow(C692, C684 - 1) /
                                  (2. * (std::pow(2., C682) - 1.)) -
                              std::exp(-0.00081290825 * std::sqrt(C690) /
                                       (std::pow(C680, 7. / 6.) * C715)) *
                                  C715 * C690 * std::pow(C680, 7. / 3.) * C685 *
                                  std::pow(C692, C685 - 1) /
                                  (4. *
                                   std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                   std::pow(
                                       std::pow(C680, 7. / 3.) *
                                           std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                     2.),
                                       2))) -
                             C680 *
                                 (4. *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                      std::pow(std::pow(C680, 7. / 3.) *
                                                   std::sqrt((std::pow(C692, C685) +
                                                              std::pow(C693, C685)) /
                                                             2.),
                                               2) *
                                      (std::exp(-0.00081290825 * std::sqrt(C690) /
                                                (std::pow(C680, 7. / 6.) * C715)) *
                                           C715 * C690 * C685 * std::pow(C692, C685 - 1) * 7. *
                                           std::pow(C680, 4. / 3.) / 3. +
                                       std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) *
                                           C690 * C715 *
                                           std::exp(-0.00081290825 * std::sqrt(C690) /
                                                    (std::pow(C680, 7. / 6.) * C715)) *
                                           0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                           std::pow(C680, 1. / 6.) /
                                           (6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2))) -
                                  std::exp(-0.00081290825 * std::sqrt(C690) /
                                           (std::pow(C680, 7. / 6.) * C715)) *
                                      C715 * C690 * std::pow(C680, 7. / 3.) * C685 *
                                      std::pow(C692, C685 - 1) * 4. *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                      std::pow(C680, 7. / 3.) *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                      2 *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                      7. * std::pow(C680, 4. / 3.) / 3.) /
                                 std::pow(
                                     4. *
                                         std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                   2.) *
                                         std::pow(std::pow(C680, 7. / 3.) *
                                                      std::sqrt((std::pow(C692, C685) +
                                                                 std::pow(C693, C685)) /
                                                                2.),
                                                  2),
                                     2)) *
                                C724) +
                           ((((((0.01555 * C703 + 0.0007 * C705) - 0.0269) - 0.0048 * C696) -
                              C716) *
                                 C684 * std::pow(C693, C684 - 1) /
                                 (2. * (std::pow(2., C682) - 1.)) -
                             std::exp(-0.00081290825 * std::sqrt(C690) /
                                      (std::pow(C680, 7. / 6.) * C715)) *
                                 C715 * C690 * std::pow(C680, 7. / 3.) * C685 *
                                 std::pow(C693, C685 - 1) /
                                 (4. *
                                  std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                  std::pow(
                                      std::pow(C680, 7. / 3.) *
                                          std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                    2.),
                                      2))) -
                            C680 *
                                (4. *
                                     std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                     std::pow(std::pow(C680, 7. / 3.) *
                                                  std::sqrt((std::pow(C692, C685) +
                                                             std::pow(C693, C685)) /
                                                            2.),
                                              2) *
                                     (std::exp(-0.00081290825 * std::sqrt(C690) /
                                               (std::pow(C680, 7. / 6.) * C715)) *
                                          C715 * C690 * C685 * std::pow(C693, C685 - 1) * 7. *
                                          std::pow(C680, 4. / 3.) / 3. +
                                      std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) *
                                          C690 * C715 *
                                          std::exp(-0.00081290825 * std::sqrt(C690) /
                                                   (std::pow(C680, 7. / 6.) * C715)) *
                                          0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                          std::pow(C680, 1. / 6.) /
                                          (6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2))) -
                                 std::exp(-0.00081290825 * std::sqrt(C690) /
                                          (std::pow(C680, 7. / 6.) * C715)) *
                                     C715 * C690 * std::pow(C680, 7. / 3.) * C685 *
                                     std::pow(C693, C685 - 1) * 4. *
                                     std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                     std::pow(C680, 7. / 3.) *
                                     std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                     2 *
                                     std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                     7. * std::pow(C680, 4. / 3.) / 3.) /
                                std::pow(
                                    4. *
                                        std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                  2.) *
                                        std::pow(std::pow(C680, 7. / 3.) *
                                                     std::sqrt((std::pow(C692, C685) +
                                                                std::pow(C693, C685)) /
                                                               2.),
                                                 2),
                                    2)) *
                               C725) +
                          (std::pow(C680, 7. / 3.) *
                               std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                               C747 *
                               (C680 *
                                    (std::exp(-0.00081290825 * std::sqrt(C690) /
                                              (std::pow(C680, 7. / 6.) * C715)) *
                                         0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                         std::pow(C680, 1. / 6.) /
                                         (6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2)) +
                                     (std::pow(std::pow(C680, 7. / 6.) * C715, 2) * C715 *
                                          (std::exp(-0.00081290825 * std::sqrt(C690) /
                                                    (std::pow(C680, 7. / 6.) * C715)) *
                                               0.00081290825 * std::sqrt(C690) * 7. *
                                               std::pow(C680, 1. / 6.) / 6. +
                                           0.00081290825 * std::sqrt(C690) *
                                               std::pow(C680, 7. / 6.) *
                                               std::exp(-0.00081290825 * std::sqrt(C690) /
                                                        (std::pow(C680, 7. / 6.) * C715)) *
                                               0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                               std::pow(C680, 1. / 6.) /
                                               (6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2))) -
                                      C715 *
                                          std::exp(-0.00081290825 * std::sqrt(C690) /
                                                   (std::pow(C680, 7. / 6.) * C715)) *
                                          0.00081290825 * std::sqrt(C690) *
                                          std::pow(C680, 7. / 6.) * std::pow(C680, 7. / 6.) * C715 *
                                          2 * C715 * 7. * std::pow(C680, 1. / 6.) / 6.) /
                                         std::pow(std::pow(C680, 7. / 6.) * C715, 4)) *
                                    C690 +
                                (std::exp(-0.00081290825 * std::sqrt(C690) /
                                          (std::pow(C680, 7. / 6.) * C715)) +
                                 C715 *
                                     std::exp(-0.00081290825 * std::sqrt(C690) /
                                              (std::pow(C680, 7. / 6.) * C715)) *
                                     0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                                     std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                                    C690) -
                           C747 * C680 *
                               (std::exp(-0.00081290825 * std::sqrt(C690) /
                                         (std::pow(C680, 7. / 6.) * C715)) +
                                C715 *
                                    std::exp(-0.00081290825 * std::sqrt(C690) /
                                             (std::pow(C680, 7. / 6.) * C715)) *
                                    0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                                    std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                               C690 *
                               std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) * 7. *
                               std::pow(C680, 4. / 3.) / 3.) /
                              std::pow(
                                  std::pow(C680, 7. / 3.) *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                  2)) *
                             C749 +
                         (((((((((std::pow(
                                      std::pow(C680, 7. / 3.) *
                                          std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                    2.),
                                      2) *
                                      C680 *
                                      (C690 * C715 *
                                           std::exp(-0.00081290825 * std::sqrt(C690) /
                                                    (std::pow(C680, 7. / 6.) * C715)) *
                                           0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                           std::pow(C680, 1. / 6.) * std::pow(C680, 7. / 3.) *
                                           C685 * std::pow(C692, C685 - 1) /
                                           (4. *
                                            std::sqrt(
                                                (std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                            6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2)) -
                                       std::exp(-0.00081290825 * std::sqrt(C690) /
                                                (std::pow(C680, 7. / 6.) * C715)) *
                                           C715 * C690 * 7. * std::pow(C680, 4. / 3.) * C685 *
                                           std::pow(C692, C685 - 1) /
                                           (12. * std::sqrt((std::pow(C692, C685) +
                                                             std::pow(C693, C685)) /
                                                            2.))) -
                                  C680 *
                                      (std::pow(C680, 7. / 3.) *
                                           std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                     2.) *
                                           C690 * C715 *
                                           std::exp(-0.00081290825 * std::sqrt(C690) /
                                                    (std::pow(C680, 7. / 6.) * C715)) *
                                           0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                           std::pow(C680, 1. / 6.) /
                                           (6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2)) -
                                       std::exp(-0.00081290825 * std::sqrt(C690) /
                                                (std::pow(C680, 7. / 6.) * C715)) *
                                           C715 * C690 *
                                           std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                     2.) *
                                           7. * std::pow(C680, 4. / 3.) / 3.) *
                                      std::pow(C680, 7. / 3.) *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                      2 * std::pow(C680, 7. / 3.) * C685 *
                                      std::pow(C692, C685 - 1) /
                                      (4. *
                                       std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                 2.))) /
                                     std::pow(std::pow(C680, 7. / 3.) *
                                                  std::sqrt((std::pow(C692, C685) +
                                                             std::pow(C693, C685)) /
                                                            2.),
                                              4) +
                                 (((((0.01555 * C703 + 0.0007 * C705) - 0.0269) - 0.0048 * C696) -
                                   C716) *
                                      C684 * std::pow(C692, C684 - 1) /
                                      (2. * (std::pow(2., C682) - 1.)) -
                                  std::exp(-0.00081290825 * std::sqrt(C690) /
                                           (std::pow(C680, 7. / 6.) * C715)) *
                                      C715 * C690 * std::pow(C680, 7. / 3.) * C685 *
                                      std::pow(C692, C685 - 1) /
                                      (4. *
                                       std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                 2.) *
                                       std::pow(std::pow(C680, 7. / 3.) *
                                                    std::sqrt((std::pow(C692, C685) +
                                                               std::pow(C693, C685)) /
                                                              2.),
                                                2)))) *
                                    C718 -
                                C748 * C680 * C684 * std::pow(C692, C684 - 1) /
                                    (2. * (std::pow(2., C682) - 1.))) +
                               C735 * C680 * 0.01555 * C684 * std::pow(C692, C684 - 1) /
                                   (2. * (std::pow(2., C682) - 1.))) +
                              C737 * C680 * 0.0007 * C684 * std::pow(C692, C684 - 1) /
                                  (2. * (std::pow(2., C682) - 1.))) +
                             C728 * C680 * -0.0048 * C684 * std::pow(C692, C684 - 1) /
                                 (2. * (std::pow(2., C682) - 1.))) +
                            C680 *
                                (((((0.01555 * C703 + 0.0007 * C705) - 0.0269) - 0.0048 * C696) -
                                  C716) *
                                     C684 * (C684 - 1) * std::pow(C692, C684 - 2) /
                                     (2. * (std::pow(2., C682) - 1.)) -
                                 (4. *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                      std::pow(std::pow(C680, 7. / 3.) *
                                                   std::sqrt((std::pow(C692, C685) +
                                                              std::pow(C693, C685)) /
                                                             2.),
                                               2) *
                                      std::exp(-0.00081290825 * std::sqrt(C690) /
                                               (std::pow(C680, 7. / 6.) * C715)) *
                                      C715 * C690 * std::pow(C680, 7. / 3.) * C685 * (C685 - 1) *
                                      std::pow(C692, C685 - 2) -
                                  std::exp(-0.00081290825 * std::sqrt(C690) /
                                           (std::pow(C680, 7. / 6.) * C715)) *
                                      C715 * C690 * std::pow(C680, 7. / 3.) * C685 *
                                      std::pow(C692, C685 - 1) * 4. *
                                      (std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                 2.) *
                                           std::pow(C680, 7. / 3.) *
                                           std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                     2.) *
                                           2 * std::pow(C680, 7. / 3.) * C685 *
                                           std::pow(C692, C685 - 1) +
                                       std::pow(std::pow(C680, 7. / 3.) *
                                                    std::sqrt((std::pow(C692, C685) +
                                                               std::pow(C693, C685)) /
                                                              2.),
                                                2) *
                                           C685 * std::pow(C692, C685 - 1)) /
                                      (4. *
                                       std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                 2.))) /
                                     std::pow(4. *
                                                  std::sqrt((std::pow(C692, C685) +
                                                             std::pow(C693, C685)) /
                                                            2.) *
                                                  std::pow(std::pow(C680, 7. / 3.) *
                                                               std::sqrt((std::pow(C692, C685) +
                                                                          std::pow(C693, C685)) /
                                                                         2.),
                                                           2),
                                              2)) *
                                C724) +
                           C725 * C680 *
                               std::exp(-0.00081290825 * std::sqrt(C690) /
                                        (std::pow(C680, 7. / 6.) * C715)) *
                               C715 * C690 * std::pow(C680, 7. / 3.) * C685 *
                               std::pow(C693, C685 - 1) * 4. *
                               (std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                    std::pow(C680, 7. / 3.) *
                                    std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                    2 * std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) +
                                std::pow(
                                    std::pow(C680, 7. / 3.) *
                                        std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                  2.),
                                    2) *
                                    C685 * std::pow(C692, C685 - 1)) /
                               (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                std::pow(
                                    4. *
                                        std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                  2.) *
                                        std::pow(std::pow(C680, 7. / 3.) *
                                                     std::sqrt((std::pow(C692, C685) +
                                                                std::pow(C693, C685)) /
                                                               2.),
                                                 2),
                                    2))) -
                          C747 * C680 *
                              (std::exp(-0.00081290825 * std::sqrt(C690) /
                                        (std::pow(C680, 7. / 6.) * C715)) +
                               C715 *
                                   std::exp(-0.00081290825 * std::sqrt(C690) /
                                            (std::pow(C680, 7. / 6.) * C715)) *
                                   0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                                   std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                              C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) /
                              (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                               std::pow(std::pow(C680, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                        2))) *
                             C755) +
                        (((((((((std::pow(
                                     std::pow(C680, 7. / 3.) *
                                         std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                   2.),
                                     2) *
                                     C680 *
                                     (C690 * C715 *
                                          std::exp(-0.00081290825 * std::sqrt(C690) /
                                                   (std::pow(C680, 7. / 6.) * C715)) *
                                          0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                          std::pow(C680, 1. / 6.) * std::pow(C680, 7. / 3.) * C685 *
                                          std::pow(C693, C685 - 1) /
                                          (4. *
                                           std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                     2.) *
                                           6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2)) -
                                      std::exp(-0.00081290825 * std::sqrt(C690) /
                                               (std::pow(C680, 7. / 6.) * C715)) *
                                          C715 * C690 * 7. * std::pow(C680, 4. / 3.) * C685 *
                                          std::pow(C693, C685 - 1) /
                                          (12. *
                                           std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                     2.))) -
                                 C680 *
                                     (std::pow(C680, 7. / 3.) *
                                          std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                    2.) *
                                          C690 * C715 *
                                          std::exp(-0.00081290825 * std::sqrt(C690) /
                                                   (std::pow(C680, 7. / 6.) * C715)) *
                                          0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                          std::pow(C680, 1. / 6.) /
                                          (6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2)) -
                                      std::exp(-0.00081290825 * std::sqrt(C690) /
                                               (std::pow(C680, 7. / 6.) * C715)) *
                                          C715 * C690 *
                                          std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                    2.) *
                                          7. * std::pow(C680, 4. / 3.) / 3.) *
                                     std::pow(C680, 7. / 3.) *
                                     std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                     2 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) /
                                     (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                     2.))) /
                                    std::pow(
                                        std::pow(C680, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                        4) +
                                (((((0.01555 * C703 + 0.0007 * C705) - 0.0269) - 0.0048 * C696) -
                                  C716) *
                                     C684 * std::pow(C693, C684 - 1) /
                                     (2. * (std::pow(2., C682) - 1.)) -
                                 std::exp(-0.00081290825 * std::sqrt(C690) /
                                          (std::pow(C680, 7. / 6.) * C715)) *
                                     C715 * C690 * std::pow(C680, 7. / 3.) * C685 *
                                     std::pow(C693, C685 - 1) /
                                     (4. *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                      std::pow(std::pow(C680, 7. / 3.) *
                                                   std::sqrt((std::pow(C692, C685) +
                                                              std::pow(C693, C685)) /
                                                             2.),
                                               2)))) *
                                   C718 -
                               C748 * C680 * C684 * std::pow(C693, C684 - 1) /
                                   (2. * (std::pow(2., C682) - 1.))) +
                              C735 * C680 * 0.01555 * C684 * std::pow(C693, C684 - 1) /
                                  (2. * (std::pow(2., C682) - 1.))) +
                             C737 * C680 * 0.0007 * C684 * std::pow(C693, C684 - 1) /
                                 (2. * (std::pow(2., C682) - 1.))) +
                            C728 * C680 * -0.0048 * C684 * std::pow(C693, C684 - 1) /
                                (2. * (std::pow(2., C682) - 1.))) +
                           C724 * C680 *
                               std::exp(-0.00081290825 * std::sqrt(C690) /
                                        (std::pow(C680, 7. / 6.) * C715)) *
                               C715 * C690 * std::pow(C680, 7. / 3.) * C685 *
                               std::pow(C692, C685 - 1) * 4. *
                               (std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                    std::pow(C680, 7. / 3.) *
                                    std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                    2 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) +
                                std::pow(
                                    std::pow(C680, 7. / 3.) *
                                        std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                  2.),
                                    2) *
                                    C685 * std::pow(C693, C685 - 1)) /
                               (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                std::pow(
                                    4. *
                                        std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                  2.) *
                                        std::pow(std::pow(C680, 7. / 3.) *
                                                     std::sqrt((std::pow(C692, C685) +
                                                                std::pow(C693, C685)) /
                                                               2.),
                                                 2),
                                    2))) +
                          C680 *
                              (((((0.01555 * C703 + 0.0007 * C705) - 0.0269) - 0.0048 * C696) -
                                C716) *
                                   C684 * (C684 - 1) * std::pow(C693, C684 - 2) /
                                   (2. * (std::pow(2., C682) - 1.)) -
                               (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                    std::pow(
                                        std::pow(C680, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                        2) *
                                    std::exp(-0.00081290825 * std::sqrt(C690) /
                                             (std::pow(C680, 7. / 6.) * C715)) *
                                    C715 * C690 * std::pow(C680, 7. / 3.) * C685 * (C685 - 1) *
                                    std::pow(C693, C685 - 2) -
                                std::exp(-0.00081290825 * std::sqrt(C690) /
                                         (std::pow(C680, 7. / 6.) * C715)) *
                                    C715 * C690 * std::pow(C680, 7. / 3.) * C685 *
                                    std::pow(C693, C685 - 1) * 4. *
                                    (std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                         std::pow(C680, 7. / 3.) *
                                         std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                   2.) *
                                         2 * std::pow(C680, 7. / 3.) * C685 *
                                         std::pow(C693, C685 - 1) +
                                     std::pow(std::pow(C680, 7. / 3.) *
                                                  std::sqrt((std::pow(C692, C685) +
                                                             std::pow(C693, C685)) /
                                                            2.),
                                              2) *
                                         C685 * std::pow(C693, C685 - 1)) /
                                    (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                    2.))) /
                                   std::pow(
                                       4. *
                                           std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                     2.) *
                                           std::pow(std::pow(C680, 7. / 3.) *
                                                        std::sqrt((std::pow(C692, C685) +
                                                                   std::pow(C693, C685)) /
                                                                  2.),
                                                    2),
                                       2)) *
                              C725) -
                         C747 * C680 *
                             (std::exp(-0.00081290825 * std::sqrt(C690) /
                                       (std::pow(C680, 7. / 6.) * C715)) +
                              C715 *
                                  std::exp(-0.00081290825 * std::sqrt(C690) /
                                           (std::pow(C680, 7. / 6.) * C715)) *
                                  0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                                  std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                             C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) /
                             (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                              std::pow(
                                  std::pow(C680, 7. / 3.) *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                  2))) *
                            C756) +
                       ((((C680 *
                               ((6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2) *
                                     std::pow(C680, 7. / 3.) *
                                     std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                     C690 *
                                     (C715 * (std::exp(-0.00081290825 * std::sqrt(C690) /
                                                       (std::pow(C680, 7. / 6.) * C715)) *
                                                  0.00081290825 * std::sqrt(C690) * 7. *
                                                  std::pow(C680, 1. / 6.) +
                                              0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                                  std::pow(C680, 1. / 6.) *
                                                  std::exp(-0.00081290825 * std::sqrt(C690) /
                                                           (std::pow(C680, 7. / 6.) * C715)) *
                                                  0.00081290825 * std::sqrt(C690) *
                                                  std::pow(C680, 7. / 6.) /
                                                  std::pow(std::pow(C680, 7. / 6.) * C715, 2)) +
                                      std::exp(-0.00081290825 * std::sqrt(C690) /
                                               (std::pow(C680, 7. / 6.) * C715)) *
                                          0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                          std::pow(C680, 1. / 6.)) -
                                 std::pow(C680, 7. / 3.) *
                                     std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                     C690 * C715 *
                                     std::exp(-0.00081290825 * std::sqrt(C690) /
                                              (std::pow(C680, 7. / 6.) * C715)) *
                                     0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                     std::pow(C680, 1. / 6.) * 12. * C715 *
                                     std::pow(C680, 14. / 6.)) /
                                    std::pow(6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2), 2) -
                                (std::exp(-0.00081290825 * std::sqrt(C690) /
                                          (std::pow(C680, 7. / 6.) * C715)) +
                                 C715 *
                                     std::exp(-0.00081290825 * std::sqrt(C690) /
                                              (std::pow(C680, 7. / 6.) * C715)) *
                                     0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                                     std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                                    C690 *
                                    std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                    7. * std::pow(C680, 4. / 3.) / 3.) /
                               std::pow(std::pow(C680, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                        2) +
                           (std::exp(-0.00081290825 * std::sqrt(C690) /
                                     (std::pow(C680, 7. / 6.) * C715)) +
                            C715 *
                                std::exp(-0.00081290825 * std::sqrt(C690) /
                                         (std::pow(C680, 7. / 6.) * C715)) *
                                0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                                std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                               C690 /
                               (std::pow(C680, 7. / 3.) *
                                std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.))) *
                              C718 -
                          C724 * C680 *
                              (std::exp(-0.00081290825 * std::sqrt(C690) /
                                        (std::pow(C680, 7. / 6.) * C715)) +
                               C715 *
                                   std::exp(-0.00081290825 * std::sqrt(C690) /
                                            (std::pow(C680, 7. / 6.) * C715)) *
                                   0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                                   std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                              C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) /
                              (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                               std::pow(std::pow(C680, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                        2))) -
                         C725 * C680 *
                             (std::exp(-0.00081290825 * std::sqrt(C690) /
                                       (std::pow(C680, 7. / 6.) * C715)) +
                              C715 *
                                  std::exp(-0.00081290825 * std::sqrt(C690) /
                                           (std::pow(C680, 7. / 6.) * C715)) *
                                  0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                                  std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                             C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) /
                             (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                              std::pow(
                                  std::pow(C680, 7. / 3.) *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                  2))) +
                        C747 * C680 *
                            (std::exp(-0.00081290825 * std::sqrt(C690) /
                                      (std::pow(C680, 7. / 6.) * C715)) *
                                 0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                                 std::pow(std::pow(C680, 7. / 6.) * C715, 2) +
                             (std::pow(std::pow(C680, 7. / 6.) * C715, 2) *
                                  (C715 * 0.00081290825 * std::sqrt(C690) *
                                       std::pow(C680, 7. / 6.) *
                                       std::exp(-0.00081290825 * std::sqrt(C690) /
                                                (std::pow(C680, 7. / 6.) * C715)) *
                                       0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                                       std::pow(std::pow(C680, 7. / 6.) * C715, 2) +
                                   std::exp(-0.00081290825 * std::sqrt(C690) /
                                            (std::pow(C680, 7. / 6.) * C715)) *
                                       0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.)) -
                              C715 *
                                  std::exp(-0.00081290825 * std::sqrt(C690) /
                                           (std::pow(C680, 7. / 6.) * C715)) *
                                  0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * 2 *
                                  C715 * std::pow(C680, 14. / 6.)) /
                                 std::pow(std::pow(C680, 7. / 6.) * C715, 4)) *
                            C690 /
                            (std::pow(C680, 7. / 3.) *
                             std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.))) *
                           C778) +
                      ((((2. - (std::pow(C692, C684) + std::pow(C693, C684))) /
                             (2. * (std::pow(2., C682) - 1.)) +
                         1) *
                            C718 -
                        C724 * C680 * C684 * std::pow(C692, C684 - 1) /
                            (2. * (std::pow(2., C682) - 1.))) -
                       C725 * C680 * C684 * std::pow(C693, C684 - 1) /
                           (2. * (std::pow(2., C682) - 1.))) *
                          C779) +
                     C766 *
                         ((C718 * 0.01555 * ((std::pow(C692, C684) + std::pow(C693, C684)) - 2.) +
                           C724 * C680 * 0.01555 * C684 * std::pow(C692, C684 - 1)) +
                          C725 * C680 * 0.01555 * C684 * std::pow(C693, C684 - 1)) /
                         (2. * (std::pow(2., C682) - 1.))) +
                    C768 *
                        ((C718 * 0.0007 * ((std::pow(C692, C684) + std::pow(C693, C684)) - 2.) +
                          C724 * C680 * 0.0007 * C684 * std::pow(C692, C684 - 1)) +
                         C725 * C680 * 0.0007 * C684 * std::pow(C693, C684 - 1)) /
                        (2. * (std::pow(2., C682) - 1.))) +
                   C759 *
                       ((C718 * -0.0048 * ((std::pow(C692, C684) + std::pow(C693, C684)) - 2.) +
                         C724 * C680 * -0.0048 * C684 * std::pow(C692, C684 - 1)) +
                        C725 * C680 * -0.0048 * C684 * std::pow(C693, C684 - 1)) /
                       (2. * (std::pow(2., C682) - 1.))) +
                  C680 *
                      ((2. - (std::pow(C692, C684) + std::pow(C693, C684))) /
                           (2. * (std::pow(2., C682) - 1.)) +
                       1) *
                      C866) +
                 C853 * C680 * 0.01555 * ((std::pow(C692, C684) + std::pow(C693, C684)) - 2.) /
                     (2. * (std::pow(2., C682) - 1.))) +
                C855 * C680 * 0.0007 * ((std::pow(C692, C684) + std::pow(C693, C684)) - 2.) /
                    (2. * (std::pow(2., C682) - 1.))) +
               C846 * C680 * -0.0048 * ((std::pow(C692, C684) + std::pow(C693, C684)) - 2.) /
                   (2. * (std::pow(2., C682) - 1.))) +
              C680 *
                  (((((0.01555 * C703 + 0.0007 * C705) - 0.0269) - 0.0048 * C696) - C716) * C684 *
                       std::pow(C692, C684 - 1) / (2. * (std::pow(2., C682) - 1.)) -
                   std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                       C715 * C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) /
                       (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                        std::pow(std::pow(C680, 7. / 3.) *
                                     std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                 2))) *
                  C842) +
             C680 *
                 (((((0.01555 * C703 + 0.0007 * C705) - 0.0269) - 0.0048 * C696) - C716) * C684 *
                      std::pow(C693, C684 - 1) / (2. * (std::pow(2., C682) - 1.)) -
                  std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                      C715 * C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) /
                      (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                       std::pow(std::pow(C680, 7. / 3.) *
                                    std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                2))) *
                 C843) +
            C865 * C680 *
                (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                 C715 *
                     std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                     0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                     std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                C690 /
                (std::pow(C680, 7. / 3.) *
                 std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.));

        const double C867 = -24. * Pi;
        const double C868 = 0.88668 * Pi;
        const double C869 = 2. * sigmaAb;
        const double C870 = 12. * Pi;
        const double C871 = rhoA + rhoB;
        const double C872 = rhoA - rhoB;
        const double C873 = -2. / 3.;
        const double C874 = -1. / 3.;
        const double C875 = 1. / 3.;
        const double C876 = 2. / 3.;
        const double C877 = 4. / 3.;
        const double C878 = 5. / 3.;
        const double C879 = 7. / 6.;
        const double C880 = 0.9999999999999999 * C871;
        const double C881 = 0.9999999999999999 * C872;
        const double C882 = Pi * C871;
        const double C883 = sigmaAa + C869;
        const double C884 = std::pow(C871, C877);
        const double C885 = std::pow(C871, C879);
        const double C886 = 4. * C882;
        const double C887 = C883 + sigmaBb;
        const double C888 = C880 - C881;
        const double C889 = C881 / C871;
        const double C890 = C889 + 1.;
        const double C891 = 1. - C889;
        const double C892 = 0.22167 / C886;
        const double C893 = 3. / C886;
        const double C894 = std::sqrt(C887);
        const double C895 = std::pow(C886, 2);
        const double C896 = 0.00081290825 * C894;
        const double C897 = 3. * C895;
        const double C898 = C868 / C895;
        const double C899 = std::pow(C890, C878);
        const double C900 = std::pow(C891, C878);
        const double C901 = std::pow(C893, C873);
        const double C902 = std::pow(C893, C874);
        const double C903 = std::pow(C893, C875);
        const double C904 = std::pow(C893, C876);
        const double C905 = 7.389e-6 * C904;
        const double C906 = 0.023266 * C903;
        const double C907 = 0.472 * C904;
        const double C908 = 8.723 * C903;
        const double C909 = C901 * C870;
        const double C910 = C902 * C867;
        const double C911 = C899 + C900;
        const double C912 = -8.723 * C909;
        const double C913 = -0.023266 * C909;
        const double C914 = 7.389e-6 * C910;
        const double C915 = 0.472 * C910;
        const double C916 = C906 + C905;
        const double C917 = C908 + C907;
        const double C918 = C911 / 2.;
        const double C919 = C912 + C915;
        const double C920 = C913 + C914;
        const double C921 = C916 + 0.002568;
        const double C922 = C917 + C892;
        const double C923 = std::sqrt(C918);
        const double C924 = C884 * C923;
        const double C925 = C922 + 1.;
        const double C926 = C919 / C897;
        const double C927 = C925 * C920;
        const double C928 = C926 - C898;
        const double C929 = C921 / C925;
        const double C930 = std::pow(C925, 2);
        const double C931 = C921 * C928;
        const double C932 = C929 + 0.001667;
        const double C933 = C927 / C897;
        const double C934 = C932 * C885;
        const double C935 = C933 - C931;
        const double C936 = C896 / C934;
        const double C937 = -C936;
        const double C938 = std::exp(C937);
        matrix.upper[2] =
            C938 * (1. - C896 / (2. * C934)) *
                (C924 * C935 / C930 - C932 * (C884 *
                                                  (std::pow(C890, C876) * 5. * C888 +
                                                   std::pow(C891, C876) * -5. * C888) /
                                                  (6. * std::pow(C871, 2) * 2 * C923) +
                                              C923 * 4. * std::pow(C871, C875) / 3.)) /
                std::pow(C924, 2) +
            C932 * 0.00081290825 * (C932 * 7. * std::pow(C871, 1. / 6.) / 6. + C885 * C935 / C930) *
                (3. - C936) * C938 * C894 / (2. * std::pow(C934, 2) * C924);

        const double C939 = -24. * Pi;
        const double C940 = 0.88668 * Pi;
        const double C941 = 2. * sigmaAb;
        const double C942 = 12. * Pi;
        const double C943 = rhoA + rhoB;
        const double C944 = rhoA - rhoB;
        const double C945 = -2. / 3.;
        const double C946 = -1. / 3.;
        const double C947 = 1. / 3.;
        const double C948 = 2. / 3.;
        const double C949 = 4. / 3.;
        const double C950 = 5. / 3.;
        const double C951 = 7. / 6.;
        const double C952 = 0.9999999999999999 * C943;
        const double C953 = 0.9999999999999999 * C944;
        const double C954 = Pi * C943;
        const double C955 = sigmaAa + C941;
        const double C956 = std::pow(C943, C949);
        const double C957 = std::pow(C943, C951);
        const double C958 = 4. * C954;
        const double C959 = C955 + sigmaBb;
        const double C960 = C952 - C953;
        const double C961 = C953 / C943;
        const double C962 = C961 + 1.;
        const double C963 = 1. - C961;
        const double C964 = 0.22167 / C958;
        const double C965 = 3. / C958;
        const double C966 = std::sqrt(C959);
        const double C967 = std::pow(C958, 2);
        const double C968 = 0.00081290825 * C966;
        const double C969 = 3. * C967;
        const double C970 = C940 / C967;
        const double C971 = std::pow(C962, C950);
        const double C972 = std::pow(C963, C950);
        const double C973 = std::pow(C965, C945);
        const double C974 = std::pow(C965, C946);
        const double C975 = std::pow(C965, C947);
        const double C976 = std::pow(C965, C948);
        const double C977 = 7.389e-6 * C976;
        const double C978 = 0.023266 * C975;
        const double C979 = 0.472 * C976;
        const double C980 = 8.723 * C975;
        const double C981 = C973 * C942;
        const double C982 = C974 * C939;
        const double C983 = C971 + C972;
        const double C984 = -8.723 * C981;
        const double C985 = -0.023266 * C981;
        const double C986 = 7.389e-6 * C982;
        const double C987 = 0.472 * C982;
        const double C988 = C978 + C977;
        const double C989 = C980 + C979;
        const double C990 = C983 / 2.;
        const double C991 = C984 + C987;
        const double C992 = C985 + C986;
        const double C993 = C988 + 0.002568;
        const double C994 = C989 + C964;
        const double C995 = std::sqrt(C990);
        const double C996 = C956 * C995;
        const double C997 = C994 + 1.;
        const double C998 = C991 / C969;
        const double C999 = C997 * C992;
        const double C1000 = C998 - C970;
        const double C1001 = C993 / C997;
        const double C1002 = std::pow(C997, 2);
        const double C1003 = C993 * C1000;
        const double C1004 = C1001 + 0.001667;
        const double C1005 = C999 / C969;
        const double C1006 = C1004 * C957;
        const double C1007 = C1005 - C1003;
        const double C1008 = C968 / C1006;
        const double C1009 = -C1008;
        const double C1010 = std::exp(C1009);
        matrix.upper[3] =
            2. * (C1010 * (1. - C968 / (2. * C1006)) *
                      (C996 * C1007 / C1002 - C1004 * (C956 *
                                                           (std::pow(C962, C948) * 5. * C960 +
                                                            std::pow(C963, C948) * -5. * C960) /
                                                           (6. * std::pow(C943, 2) * 2 * C995) +
                                                       C995 * 4. * std::pow(C943, C947) / 3.)) /
                      std::pow(C996, 2) +
                  C1004 * 0.00081290825 *
                      (C1004 * 7. * std::pow(C943, 1. / 6.) / 6. + C957 * C1007 / C1002) *
                      (3. - C1008) * C1010 * C966 / (2. * std::pow(C1006, 2) * C996));

        const double C1011 = -24. * Pi;
        const double C1012 = 0.88668 * Pi;
        const double C1013 = 2. * sigmaAb;
        const double C1014 = 12. * Pi;
        const double C1015 = rhoA + rhoB;
        const double C1016 = rhoA - rhoB;
        const double C1017 = -2. / 3.;
        const double C1018 = -1. / 3.;
        const double C1019 = 1. / 3.;
        const double C1020 = 2. / 3.;
        const double C1021 = 4. / 3.;
        const double C1022 = 5. / 3.;
        const double C1023 = 7. / 6.;
        const double C1024 = 0.9999999999999999 * C1015;
        const double C1025 = 0.9999999999999999 * C1016;
        const double C1026 = Pi * C1015;
        const double C1027 = sigmaAa + C1013;
        const double C1028 = std::pow(C1015, C1021);
        const double C1029 = std::pow(C1015, C1023);
        const double C1030 = 4. * C1026;
        const double C1031 = C1027 + sigmaBb;
        const double C1032 = C1024 - C1025;
        const double C1033 = C1025 / C1015;
        const double C1034 = C1033 + 1.;
        const double C1035 = 1. - C1033;
        const double C1036 = 0.22167 / C1030;
        const double C1037 = 3. / C1030;
        const double C1038 = std::sqrt(C1031);
        const double C1039 = std::pow(C1030, 2);
        const double C1040 = 0.00081290825 * C1038;
        const double C1041 = 3. * C1039;
        const double C1042 = C1012 / C1039;
        const double C1043 = std::pow(C1034, C1022);
        const double C1044 = std::pow(C1035, C1022);
        const double C1045 = std::pow(C1037, C1017);
        const double C1046 = std::pow(C1037, C1018);
        const double C1047 = std::pow(C1037, C1019);
        const double C1048 = std::pow(C1037, C1020);
        const double C1049 = 7.389e-6 * C1048;
        const double C1050 = 0.023266 * C1047;
        const double C1051 = 0.472 * C1048;
        const double C1052 = 8.723 * C1047;
        const double C1053 = C1045 * C1014;
        const double C1054 = C1046 * C1011;
        const double C1055 = C1043 + C1044;
        const double C1056 = -8.723 * C1053;
        const double C1057 = -0.023266 * C1053;
        const double C1058 = 7.389e-6 * C1054;
        const double C1059 = 0.472 * C1054;
        const double C1060 = C1050 + C1049;
        const double C1061 = C1052 + C1051;
        const double C1062 = C1055 / 2.;
        const double C1063 = C1056 + C1059;
        const double C1064 = C1057 + C1058;
        const double C1065 = C1060 + 0.002568;
        const double C1066 = C1061 + C1036;
        const double C1067 = std::sqrt(C1062);
        const double C1068 = C1028 * C1067;
        const double C1069 = C1066 + 1.;
        const double C1070 = C1063 / C1041;
        const double C1071 = C1069 * C1064;
        const double C1072 = C1070 - C1042;
        const double C1073 = C1065 / C1069;
        const double C1074 = std::pow(C1069, 2);
        const double C1075 = C1065 * C1072;
        const double C1076 = C1073 + 0.001667;
        const double C1077 = C1071 / C1041;
        const double C1078 = C1076 * C1029;
        const double C1079 = C1077 - C1075;
        const double C1080 = C1040 / C1078;
        const double C1081 = -C1080;
        const double C1082 = std::exp(C1081);
        matrix.upper[4] =
            C1082 * (1. - C1040 / (2. * C1078)) *
                (C1068 * C1079 / C1074 - C1076 * (C1028 *
                                                      (std::pow(C1034, C1020) * 5. * C1032 +
                                                       std::pow(C1035, C1020) * -5. * C1032) /
                                                      (6. * std::pow(C1015, 2) * 2 * C1067) +
                                                  C1067 * 4. * std::pow(C1015, C1019) / 3.)) /
                std::pow(C1068, 2) +
            C1076 * 0.00081290825 *
                (C1076 * 7. * std::pow(C1015, 1. / 6.) / 6. + C1029 * C1079 / C1074) *
                (3. - C1080) * C1082 * C1038 / (2. * std::pow(C1078, 2) * C1068);

        const double C1083 = 0.;
        const double C1084 =
            (C1083 / C680 +
             ((-C751) / std::pow(C680, 2) - (-C749 * C686 * 2 * C680) / std::pow(C680, 4)) * C749) -
            C751 * C749 / std::pow(C680, 2);
        const double C1085 = C1084;
        const double C1086 = -C1084;
        const double C1087 = (-C753 * -0.22167 * C753 * 2 * C689) / std::pow(C689, 4);
        const double C1088 = (-C753 * -3. * C753 * 2 * C689) / std::pow(C689, 4);
        const double C1089 = C682 * (C682 - 1) * std::pow(C695, C682 - 2) * std::pow(C758, 2) +
                             C682 * std::pow(C695, C682 - 1) * C1088;
        const double C1090 = C683 * (C683 - 1) * std::pow(C695, C683 - 2) * std::pow(C758, 2) +
                             C683 * std::pow(C695, C683 - 1) * C1088;
        const double C1091 = 0.7389e-5 * C1090 + 0.;
        const double C1092 = 0.0116 * C1089;
        const double C1093 = 0.023266 * C1089;
        const double C1094 = 0.472 * C1090;
        const double C1095 = 8.723 * C1089;
        const double C1096 = C1089 / C696 - std::pow(C759, 2) / std::pow(C696, 2);
        const double C1097 = 0.0311 * C1096;
        const double C1098 = ((C759 * C766 + C703 * C1089) + C766 * C759) + C696 * C1096;
        const double C1099 = C1093 + C1091;
        const double C1100 = C1095 + C1094;
        const double C1101 = 0.002 * C1098;
        const double C1102 = C1099;
        const double C1103 = C1100 + C1087;
        const double C1104 = C1097 + C1101;
        const double C1105 = C1103;
        const double C1106 = C1104;
        const double C1107 = ((C1102 / C712 + ((-C772) / std::pow(C712, 2) -
                                               (-C775 * C709 * 2 * C712) / std::pow(C712, 4)) *
                                                  C775) -
                              C1105 * C709 / std::pow(C712, 2)) -
                             C772 * C775 / std::pow(C712, 2);
        const double C1108 = C1107;
        const double C1109 = C1106 - C1092;
        matrix.upper[5] =
            ((((((((((((((((((((((std::pow(
                                      std::pow(C680, 7. / 3.) *
                                          std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                    2.),
                                      2) *
                                      (C680 *
                                           ((6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2) *
                                                 (std::pow(C680, 7. / 3.) *
                                                      std::sqrt((std::pow(C692, C685) +
                                                                 std::pow(C693, C685)) /
                                                                2.) *
                                                      C690 * C715 *
                                                      (std::exp(-0.00081290825 * std::sqrt(C690) /
                                                                (std::pow(C680, 7. / 6.) * C715)) *
                                                           0.00081290825 * std::sqrt(C690) * C715 *
                                                           7. * std::pow(C680, -5. / 6.) / 6. +
                                                       0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                                           std::pow(C680, 1. / 6.) *
                                                           std::exp(
                                                               -0.00081290825 * std::sqrt(C690) /
                                                               (std::pow(C680, 7. / 6.) * C715)) *
                                                           0.00081290825 * std::sqrt(C690) * C715 *
                                                           7. * std::pow(C680, 1. / 6.) /
                                                           (6. *
                                                            std::pow(std::pow(C680, 7. / 6.) * C715,
                                                                     2))) +
                                                  C690 * C715 *
                                                      std::exp(-0.00081290825 * std::sqrt(C690) /
                                                               (std::pow(C680, 7. / 6.) * C715)) *
                                                      0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                                      std::pow(C680, 1. / 6.) *
                                                      std::sqrt((std::pow(C692, C685) +
                                                                 std::pow(C693, C685)) /
                                                                2.) *
                                                      7. * std::pow(C680, 4. / 3.) / 3.) -
                                             std::pow(C680, 7. / 3.) *
                                                 std::sqrt(
                                                     (std::pow(C692, C685) + std::pow(C693, C685)) /
                                                     2.) *
                                                 C690 * C715 *
                                                 std::exp(-0.00081290825 * std::sqrt(C690) /
                                                          (std::pow(C680, 7. / 6.) * C715)) *
                                                 0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                                 std::pow(C680, 1. / 6.) * 6. *
                                                 std::pow(C680, 7. / 6.) * C715 * 2 * C715 * 7. *
                                                 std::pow(C680, 1. / 6.) / 6.) /
                                                std::pow(
                                                    6. *
                                                        std::pow(std::pow(C680, 7. / 6.) * C715, 2),
                                                    2) -
                                            (std::exp(-0.00081290825 * std::sqrt(C690) /
                                                      (std::pow(C680, 7. / 6.) * C715)) *
                                                 C715 * C690 *
                                                 std::sqrt(
                                                     (std::pow(C692, C685) + std::pow(C693, C685)) /
                                                     2.) *
                                                 28. * std::pow(C680, 1. / 3.) / 3. +
                                             std::sqrt(
                                                 (std::pow(C692, C685) + std::pow(C693, C685)) /
                                                 2.) *
                                                 7. * std::pow(C680, 4. / 3.) * C690 * C715 *
                                                 std::exp(-0.00081290825 * std::sqrt(C690) /
                                                          (std::pow(C680, 7. / 6.) * C715)) *
                                                 0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                                 std::pow(C680, 1. / 6.) /
                                                 (6. *
                                                  std::pow(std::pow(C680, 7. / 6.) * C715, 2))) /
                                                3.) +
                                       (std::pow(C680, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                            C690 * C715 *
                                            std::exp(-0.00081290825 * std::sqrt(C690) /
                                                     (std::pow(C680, 7. / 6.) * C715)) *
                                            0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                            std::pow(C680, 1. / 6.) /
                                            (6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2)) -
                                        std::exp(-0.00081290825 * std::sqrt(C690) /
                                                 (std::pow(C680, 7. / 6.) * C715)) *
                                            C715 * C690 *
                                            std::sqrt(
                                                (std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                            7. * std::pow(C680, 4. / 3.) / 3.)) -
                                  C680 *
                                      (std::pow(C680, 7. / 3.) *
                                           std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                     2.) *
                                           C690 * C715 *
                                           std::exp(-0.00081290825 * std::sqrt(C690) /
                                                    (std::pow(C680, 7. / 6.) * C715)) *
                                           0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                           std::pow(C680, 1. / 6.) /
                                           (6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2)) -
                                       std::exp(-0.00081290825 * std::sqrt(C690) /
                                                (std::pow(C680, 7. / 6.) * C715)) *
                                           C715 * C690 *
                                           std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                     2.) *
                                           7. * std::pow(C680, 4. / 3.) / 3.) *
                                      std::pow(C680, 7. / 3.) *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                      2 *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                      7. * std::pow(C680, 4. / 3.) / 3.) /
                                     std::pow(std::pow(C680, 7. / 3.) *
                                                  std::sqrt((std::pow(C692, C685) +
                                                             std::pow(C693, C685)) /
                                                            2.),
                                              4) +
                                 (std::pow(C680, 7. / 3.) *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                      C690 * C715 *
                                      std::exp(-0.00081290825 * std::sqrt(C690) /
                                               (std::pow(C680, 7. / 6.) * C715)) *
                                      0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                      std::pow(C680, 1. / 6.) /
                                      (6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2)) -
                                  std::exp(-0.00081290825 * std::sqrt(C690) /
                                           (std::pow(C680, 7. / 6.) * C715)) *
                                      C715 * C690 *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                      7. * std::pow(C680, 4. / 3.) / 3.) /
                                     std::pow(std::pow(C680, 7. / 3.) *
                                                  std::sqrt((std::pow(C692, C685) +
                                                             std::pow(C693, C685)) /
                                                            2.),
                                              2)) *
                                    C749 +
                                ((2. - (std::pow(C692, C684) + std::pow(C693, C684))) /
                                     (2. * (std::pow(2., C682) - 1.)) +
                                 1) *
                                    C779) +
                               C766 * 0.01555 *
                                   ((std::pow(C692, C684) + std::pow(C693, C684)) - 2.) /
                                   (2. * (std::pow(2., C682) - 1.))) +
                              C768 * 0.0007 * ((std::pow(C692, C684) + std::pow(C693, C684)) - 2.) /
                                  (2. * (std::pow(2., C682) - 1.))) +
                             C759 * -0.0048 * ((std::pow(C692, C684) + std::pow(C693, C684)) - 2.) /
                                 (2. * (std::pow(2., C682) - 1.))) +
                            ((((((0.01555 * C703 + 0.0007 * C705) - 0.0269) - 0.0048 * C696) -
                               C716) *
                                  C684 * std::pow(C692, C684 - 1) /
                                  (2. * (std::pow(2., C682) - 1.)) -
                              std::exp(-0.00081290825 * std::sqrt(C690) /
                                       (std::pow(C680, 7. / 6.) * C715)) *
                                  C715 * C690 * std::pow(C680, 7. / 3.) * C685 *
                                  std::pow(C692, C685 - 1) /
                                  (4. *
                                   std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                   std::pow(
                                       std::pow(C680, 7. / 3.) *
                                           std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                     2.),
                                       2))) -
                             C680 *
                                 (4. *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                      std::pow(std::pow(C680, 7. / 3.) *
                                                   std::sqrt((std::pow(C692, C685) +
                                                              std::pow(C693, C685)) /
                                                             2.),
                                               2) *
                                      (std::exp(-0.00081290825 * std::sqrt(C690) /
                                                (std::pow(C680, 7. / 6.) * C715)) *
                                           C715 * C690 * C685 * std::pow(C692, C685 - 1) * 7. *
                                           std::pow(C680, 4. / 3.) / 3. +
                                       std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) *
                                           C690 * C715 *
                                           std::exp(-0.00081290825 * std::sqrt(C690) /
                                                    (std::pow(C680, 7. / 6.) * C715)) *
                                           0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                           std::pow(C680, 1. / 6.) /
                                           (6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2))) -
                                  std::exp(-0.00081290825 * std::sqrt(C690) /
                                           (std::pow(C680, 7. / 6.) * C715)) *
                                      C715 * C690 * std::pow(C680, 7. / 3.) * C685 *
                                      std::pow(C692, C685 - 1) * 4. *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                      std::pow(C680, 7. / 3.) *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                      2 *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                      7. * std::pow(C680, 4. / 3.) / 3.) /
                                 std::pow(
                                     4. *
                                         std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                   2.) *
                                         std::pow(std::pow(C680, 7. / 3.) *
                                                      std::sqrt((std::pow(C692, C685) +
                                                                 std::pow(C693, C685)) /
                                                                2.),
                                                  2),
                                     2)) *
                                C755) +
                           ((((((0.01555 * C703 + 0.0007 * C705) - 0.0269) - 0.0048 * C696) -
                              C716) *
                                 C684 * std::pow(C693, C684 - 1) /
                                 (2. * (std::pow(2., C682) - 1.)) -
                             std::exp(-0.00081290825 * std::sqrt(C690) /
                                      (std::pow(C680, 7. / 6.) * C715)) *
                                 C715 * C690 * std::pow(C680, 7. / 3.) * C685 *
                                 std::pow(C693, C685 - 1) /
                                 (4. *
                                  std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                  std::pow(
                                      std::pow(C680, 7. / 3.) *
                                          std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                    2.),
                                      2))) -
                            C680 *
                                (4. *
                                     std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                     std::pow(std::pow(C680, 7. / 3.) *
                                                  std::sqrt((std::pow(C692, C685) +
                                                             std::pow(C693, C685)) /
                                                            2.),
                                              2) *
                                     (std::exp(-0.00081290825 * std::sqrt(C690) /
                                               (std::pow(C680, 7. / 6.) * C715)) *
                                          C715 * C690 * C685 * std::pow(C693, C685 - 1) * 7. *
                                          std::pow(C680, 4. / 3.) / 3. +
                                      std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) *
                                          C690 * C715 *
                                          std::exp(-0.00081290825 * std::sqrt(C690) /
                                                   (std::pow(C680, 7. / 6.) * C715)) *
                                          0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                          std::pow(C680, 1. / 6.) /
                                          (6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2))) -
                                 std::exp(-0.00081290825 * std::sqrt(C690) /
                                          (std::pow(C680, 7. / 6.) * C715)) *
                                     C715 * C690 * std::pow(C680, 7. / 3.) * C685 *
                                     std::pow(C693, C685 - 1) * 4. *
                                     std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                     std::pow(C680, 7. / 3.) *
                                     std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                     2 *
                                     std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                     7. * std::pow(C680, 4. / 3.) / 3.) /
                                std::pow(
                                    4. *
                                        std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                  2.) *
                                        std::pow(std::pow(C680, 7. / 3.) *
                                                     std::sqrt((std::pow(C692, C685) +
                                                                std::pow(C693, C685)) /
                                                               2.),
                                                 2),
                                    2)) *
                               C756) +
                          (std::pow(C680, 7. / 3.) *
                               std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                               C778 *
                               (C680 *
                                    (std::exp(-0.00081290825 * std::sqrt(C690) /
                                              (std::pow(C680, 7. / 6.) * C715)) *
                                         0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                         std::pow(C680, 1. / 6.) /
                                         (6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2)) +
                                     (std::pow(std::pow(C680, 7. / 6.) * C715, 2) * C715 *
                                          (std::exp(-0.00081290825 * std::sqrt(C690) /
                                                    (std::pow(C680, 7. / 6.) * C715)) *
                                               0.00081290825 * std::sqrt(C690) * 7. *
                                               std::pow(C680, 1. / 6.) / 6. +
                                           0.00081290825 * std::sqrt(C690) *
                                               std::pow(C680, 7. / 6.) *
                                               std::exp(-0.00081290825 * std::sqrt(C690) /
                                                        (std::pow(C680, 7. / 6.) * C715)) *
                                               0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                               std::pow(C680, 1. / 6.) /
                                               (6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2))) -
                                      C715 *
                                          std::exp(-0.00081290825 * std::sqrt(C690) /
                                                   (std::pow(C680, 7. / 6.) * C715)) *
                                          0.00081290825 * std::sqrt(C690) *
                                          std::pow(C680, 7. / 6.) * std::pow(C680, 7. / 6.) * C715 *
                                          2 * C715 * 7. * std::pow(C680, 1. / 6.) / 6.) /
                                         std::pow(std::pow(C680, 7. / 6.) * C715, 4)) *
                                    C690 +
                                (std::exp(-0.00081290825 * std::sqrt(C690) /
                                          (std::pow(C680, 7. / 6.) * C715)) +
                                 C715 *
                                     std::exp(-0.00081290825 * std::sqrt(C690) /
                                              (std::pow(C680, 7. / 6.) * C715)) *
                                     0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                                     std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                                    C690) -
                           C778 * C680 *
                               (std::exp(-0.00081290825 * std::sqrt(C690) /
                                         (std::pow(C680, 7. / 6.) * C715)) +
                                C715 *
                                    std::exp(-0.00081290825 * std::sqrt(C690) /
                                             (std::pow(C680, 7. / 6.) * C715)) *
                                    0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                                    std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                               C690 *
                               std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) * 7. *
                               std::pow(C680, 4. / 3.) / 3.) /
                              std::pow(
                                  std::pow(C680, 7. / 3.) *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                  2)) *
                             C749 +
                         (((((((((std::pow(
                                      std::pow(C680, 7. / 3.) *
                                          std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                    2.),
                                      2) *
                                      C680 *
                                      (C690 * C715 *
                                           std::exp(-0.00081290825 * std::sqrt(C690) /
                                                    (std::pow(C680, 7. / 6.) * C715)) *
                                           0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                           std::pow(C680, 1. / 6.) * std::pow(C680, 7. / 3.) *
                                           C685 * std::pow(C692, C685 - 1) /
                                           (4. *
                                            std::sqrt(
                                                (std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                            6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2)) -
                                       std::exp(-0.00081290825 * std::sqrt(C690) /
                                                (std::pow(C680, 7. / 6.) * C715)) *
                                           C715 * C690 * 7. * std::pow(C680, 4. / 3.) * C685 *
                                           std::pow(C692, C685 - 1) /
                                           (12. * std::sqrt((std::pow(C692, C685) +
                                                             std::pow(C693, C685)) /
                                                            2.))) -
                                  C680 *
                                      (std::pow(C680, 7. / 3.) *
                                           std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                     2.) *
                                           C690 * C715 *
                                           std::exp(-0.00081290825 * std::sqrt(C690) /
                                                    (std::pow(C680, 7. / 6.) * C715)) *
                                           0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                           std::pow(C680, 1. / 6.) /
                                           (6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2)) -
                                       std::exp(-0.00081290825 * std::sqrt(C690) /
                                                (std::pow(C680, 7. / 6.) * C715)) *
                                           C715 * C690 *
                                           std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                     2.) *
                                           7. * std::pow(C680, 4. / 3.) / 3.) *
                                      std::pow(C680, 7. / 3.) *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                      2 * std::pow(C680, 7. / 3.) * C685 *
                                      std::pow(C692, C685 - 1) /
                                      (4. *
                                       std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                 2.))) /
                                     std::pow(std::pow(C680, 7. / 3.) *
                                                  std::sqrt((std::pow(C692, C685) +
                                                             std::pow(C693, C685)) /
                                                            2.),
                                              4) +
                                 (((((0.01555 * C703 + 0.0007 * C705) - 0.0269) - 0.0048 * C696) -
                                   C716) *
                                      C684 * std::pow(C692, C684 - 1) /
                                      (2. * (std::pow(2., C682) - 1.)) -
                                  std::exp(-0.00081290825 * std::sqrt(C690) /
                                           (std::pow(C680, 7. / 6.) * C715)) *
                                      C715 * C690 * std::pow(C680, 7. / 3.) * C685 *
                                      std::pow(C692, C685 - 1) /
                                      (4. *
                                       std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                 2.) *
                                       std::pow(std::pow(C680, 7. / 3.) *
                                                    std::sqrt((std::pow(C692, C685) +
                                                               std::pow(C693, C685)) /
                                                              2.),
                                                2)))) *
                                    C749 -
                                C779 * C680 * C684 * std::pow(C692, C684 - 1) /
                                    (2. * (std::pow(2., C682) - 1.))) +
                               C766 * C680 * 0.01555 * C684 * std::pow(C692, C684 - 1) /
                                   (2. * (std::pow(2., C682) - 1.))) +
                              C768 * C680 * 0.0007 * C684 * std::pow(C692, C684 - 1) /
                                  (2. * (std::pow(2., C682) - 1.))) +
                             C759 * C680 * -0.0048 * C684 * std::pow(C692, C684 - 1) /
                                 (2. * (std::pow(2., C682) - 1.))) +
                            C680 *
                                (((((0.01555 * C703 + 0.0007 * C705) - 0.0269) - 0.0048 * C696) -
                                  C716) *
                                     C684 * (C684 - 1) * std::pow(C692, C684 - 2) /
                                     (2. * (std::pow(2., C682) - 1.)) -
                                 (4. *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                      std::pow(std::pow(C680, 7. / 3.) *
                                                   std::sqrt((std::pow(C692, C685) +
                                                              std::pow(C693, C685)) /
                                                             2.),
                                               2) *
                                      std::exp(-0.00081290825 * std::sqrt(C690) /
                                               (std::pow(C680, 7. / 6.) * C715)) *
                                      C715 * C690 * std::pow(C680, 7. / 3.) * C685 * (C685 - 1) *
                                      std::pow(C692, C685 - 2) -
                                  std::exp(-0.00081290825 * std::sqrt(C690) /
                                           (std::pow(C680, 7. / 6.) * C715)) *
                                      C715 * C690 * std::pow(C680, 7. / 3.) * C685 *
                                      std::pow(C692, C685 - 1) * 4. *
                                      (std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                 2.) *
                                           std::pow(C680, 7. / 3.) *
                                           std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                     2.) *
                                           2 * std::pow(C680, 7. / 3.) * C685 *
                                           std::pow(C692, C685 - 1) +
                                       std::pow(std::pow(C680, 7. / 3.) *
                                                    std::sqrt((std::pow(C692, C685) +
                                                               std::pow(C693, C685)) /
                                                              2.),
                                                2) *
                                           C685 * std::pow(C692, C685 - 1)) /
                                      (4. *
                                       std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                 2.))) /
                                     std::pow(4. *
                                                  std::sqrt((std::pow(C692, C685) +
                                                             std::pow(C693, C685)) /
                                                            2.) *
                                                  std::pow(std::pow(C680, 7. / 3.) *
                                                               std::sqrt((std::pow(C692, C685) +
                                                                          std::pow(C693, C685)) /
                                                                         2.),
                                                           2),
                                              2)) *
                                C755) +
                           C756 * C680 *
                               std::exp(-0.00081290825 * std::sqrt(C690) /
                                        (std::pow(C680, 7. / 6.) * C715)) *
                               C715 * C690 * std::pow(C680, 7. / 3.) * C685 *
                               std::pow(C693, C685 - 1) * 4. *
                               (std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                    std::pow(C680, 7. / 3.) *
                                    std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                    2 * std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) +
                                std::pow(
                                    std::pow(C680, 7. / 3.) *
                                        std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                  2.),
                                    2) *
                                    C685 * std::pow(C692, C685 - 1)) /
                               (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                std::pow(
                                    4. *
                                        std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                  2.) *
                                        std::pow(std::pow(C680, 7. / 3.) *
                                                     std::sqrt((std::pow(C692, C685) +
                                                                std::pow(C693, C685)) /
                                                               2.),
                                                 2),
                                    2))) -
                          C778 * C680 *
                              (std::exp(-0.00081290825 * std::sqrt(C690) /
                                        (std::pow(C680, 7. / 6.) * C715)) +
                               C715 *
                                   std::exp(-0.00081290825 * std::sqrt(C690) /
                                            (std::pow(C680, 7. / 6.) * C715)) *
                                   0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                                   std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                              C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) /
                              (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                               std::pow(std::pow(C680, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                        2))) *
                             C755) +
                        (((((((((std::pow(
                                     std::pow(C680, 7. / 3.) *
                                         std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                   2.),
                                     2) *
                                     C680 *
                                     (C690 * C715 *
                                          std::exp(-0.00081290825 * std::sqrt(C690) /
                                                   (std::pow(C680, 7. / 6.) * C715)) *
                                          0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                          std::pow(C680, 1. / 6.) * std::pow(C680, 7. / 3.) * C685 *
                                          std::pow(C693, C685 - 1) /
                                          (4. *
                                           std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                     2.) *
                                           6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2)) -
                                      std::exp(-0.00081290825 * std::sqrt(C690) /
                                               (std::pow(C680, 7. / 6.) * C715)) *
                                          C715 * C690 * 7. * std::pow(C680, 4. / 3.) * C685 *
                                          std::pow(C693, C685 - 1) /
                                          (12. *
                                           std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                     2.))) -
                                 C680 *
                                     (std::pow(C680, 7. / 3.) *
                                          std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                    2.) *
                                          C690 * C715 *
                                          std::exp(-0.00081290825 * std::sqrt(C690) /
                                                   (std::pow(C680, 7. / 6.) * C715)) *
                                          0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                          std::pow(C680, 1. / 6.) /
                                          (6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2)) -
                                      std::exp(-0.00081290825 * std::sqrt(C690) /
                                               (std::pow(C680, 7. / 6.) * C715)) *
                                          C715 * C690 *
                                          std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                    2.) *
                                          7. * std::pow(C680, 4. / 3.) / 3.) *
                                     std::pow(C680, 7. / 3.) *
                                     std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                     2 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) /
                                     (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                     2.))) /
                                    std::pow(
                                        std::pow(C680, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                        4) +
                                (((((0.01555 * C703 + 0.0007 * C705) - 0.0269) - 0.0048 * C696) -
                                  C716) *
                                     C684 * std::pow(C693, C684 - 1) /
                                     (2. * (std::pow(2., C682) - 1.)) -
                                 std::exp(-0.00081290825 * std::sqrt(C690) /
                                          (std::pow(C680, 7. / 6.) * C715)) *
                                     C715 * C690 * std::pow(C680, 7. / 3.) * C685 *
                                     std::pow(C693, C685 - 1) /
                                     (4. *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                2.) *
                                      std::pow(std::pow(C680, 7. / 3.) *
                                                   std::sqrt((std::pow(C692, C685) +
                                                              std::pow(C693, C685)) /
                                                             2.),
                                               2)))) *
                                   C749 -
                               C779 * C680 * C684 * std::pow(C693, C684 - 1) /
                                   (2. * (std::pow(2., C682) - 1.))) +
                              C766 * C680 * 0.01555 * C684 * std::pow(C693, C684 - 1) /
                                  (2. * (std::pow(2., C682) - 1.))) +
                             C768 * C680 * 0.0007 * C684 * std::pow(C693, C684 - 1) /
                                 (2. * (std::pow(2., C682) - 1.))) +
                            C759 * C680 * -0.0048 * C684 * std::pow(C693, C684 - 1) /
                                (2. * (std::pow(2., C682) - 1.))) +
                           C755 * C680 *
                               std::exp(-0.00081290825 * std::sqrt(C690) /
                                        (std::pow(C680, 7. / 6.) * C715)) *
                               C715 * C690 * std::pow(C680, 7. / 3.) * C685 *
                               std::pow(C692, C685 - 1) * 4. *
                               (std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                    std::pow(C680, 7. / 3.) *
                                    std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                    2 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) +
                                std::pow(
                                    std::pow(C680, 7. / 3.) *
                                        std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                  2.),
                                    2) *
                                    C685 * std::pow(C693, C685 - 1)) /
                               (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                std::pow(
                                    4. *
                                        std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                  2.) *
                                        std::pow(std::pow(C680, 7. / 3.) *
                                                     std::sqrt((std::pow(C692, C685) +
                                                                std::pow(C693, C685)) /
                                                               2.),
                                                 2),
                                    2))) +
                          C680 *
                              (((((0.01555 * C703 + 0.0007 * C705) - 0.0269) - 0.0048 * C696) -
                                C716) *
                                   C684 * (C684 - 1) * std::pow(C693, C684 - 2) /
                                   (2. * (std::pow(2., C682) - 1.)) -
                               (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                    std::pow(
                                        std::pow(C680, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                        2) *
                                    std::exp(-0.00081290825 * std::sqrt(C690) /
                                             (std::pow(C680, 7. / 6.) * C715)) *
                                    C715 * C690 * std::pow(C680, 7. / 3.) * C685 * (C685 - 1) *
                                    std::pow(C693, C685 - 2) -
                                std::exp(-0.00081290825 * std::sqrt(C690) /
                                         (std::pow(C680, 7. / 6.) * C715)) *
                                    C715 * C690 * std::pow(C680, 7. / 3.) * C685 *
                                    std::pow(C693, C685 - 1) * 4. *
                                    (std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                         std::pow(C680, 7. / 3.) *
                                         std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                   2.) *
                                         2 * std::pow(C680, 7. / 3.) * C685 *
                                         std::pow(C693, C685 - 1) +
                                     std::pow(std::pow(C680, 7. / 3.) *
                                                  std::sqrt((std::pow(C692, C685) +
                                                             std::pow(C693, C685)) /
                                                            2.),
                                              2) *
                                         C685 * std::pow(C693, C685 - 1)) /
                                    (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                    2.))) /
                                   std::pow(
                                       4. *
                                           std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                     2.) *
                                           std::pow(std::pow(C680, 7. / 3.) *
                                                        std::sqrt((std::pow(C692, C685) +
                                                                   std::pow(C693, C685)) /
                                                                  2.),
                                                    2),
                                       2)) *
                              C756) -
                         C778 * C680 *
                             (std::exp(-0.00081290825 * std::sqrt(C690) /
                                       (std::pow(C680, 7. / 6.) * C715)) +
                              C715 *
                                  std::exp(-0.00081290825 * std::sqrt(C690) /
                                           (std::pow(C680, 7. / 6.) * C715)) *
                                  0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                                  std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                             C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) /
                             (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                              std::pow(
                                  std::pow(C680, 7. / 3.) *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                  2))) *
                            C756) +
                       ((((C680 *
                               ((6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2) *
                                     std::pow(C680, 7. / 3.) *
                                     std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                     C690 *
                                     (C715 * (std::exp(-0.00081290825 * std::sqrt(C690) /
                                                       (std::pow(C680, 7. / 6.) * C715)) *
                                                  0.00081290825 * std::sqrt(C690) * 7. *
                                                  std::pow(C680, 1. / 6.) +
                                              0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                                  std::pow(C680, 1. / 6.) *
                                                  std::exp(-0.00081290825 * std::sqrt(C690) /
                                                           (std::pow(C680, 7. / 6.) * C715)) *
                                                  0.00081290825 * std::sqrt(C690) *
                                                  std::pow(C680, 7. / 6.) /
                                                  std::pow(std::pow(C680, 7. / 6.) * C715, 2)) +
                                      std::exp(-0.00081290825 * std::sqrt(C690) /
                                               (std::pow(C680, 7. / 6.) * C715)) *
                                          0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                          std::pow(C680, 1. / 6.)) -
                                 std::pow(C680, 7. / 3.) *
                                     std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                     C690 * C715 *
                                     std::exp(-0.00081290825 * std::sqrt(C690) /
                                              (std::pow(C680, 7. / 6.) * C715)) *
                                     0.00081290825 * std::sqrt(C690) * C715 * 7. *
                                     std::pow(C680, 1. / 6.) * 12. * C715 *
                                     std::pow(C680, 14. / 6.)) /
                                    std::pow(6. * std::pow(std::pow(C680, 7. / 6.) * C715, 2), 2) -
                                (std::exp(-0.00081290825 * std::sqrt(C690) /
                                          (std::pow(C680, 7. / 6.) * C715)) +
                                 C715 *
                                     std::exp(-0.00081290825 * std::sqrt(C690) /
                                              (std::pow(C680, 7. / 6.) * C715)) *
                                     0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                                     std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                                    C690 *
                                    std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                    7. * std::pow(C680, 4. / 3.) / 3.) /
                               std::pow(std::pow(C680, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                        2) +
                           (std::exp(-0.00081290825 * std::sqrt(C690) /
                                     (std::pow(C680, 7. / 6.) * C715)) +
                            C715 *
                                std::exp(-0.00081290825 * std::sqrt(C690) /
                                         (std::pow(C680, 7. / 6.) * C715)) *
                                0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                                std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                               C690 /
                               (std::pow(C680, 7. / 3.) *
                                std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.))) *
                              C749 -
                          C755 * C680 *
                              (std::exp(-0.00081290825 * std::sqrt(C690) /
                                        (std::pow(C680, 7. / 6.) * C715)) +
                               C715 *
                                   std::exp(-0.00081290825 * std::sqrt(C690) /
                                            (std::pow(C680, 7. / 6.) * C715)) *
                                   0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                                   std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                              C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) /
                              (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                               std::pow(std::pow(C680, 7. / 3.) *
                                            std::sqrt(
                                                (std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                        2))) -
                         C756 * C680 *
                             (std::exp(-0.00081290825 * std::sqrt(C690) /
                                       (std::pow(C680, 7. / 6.) * C715)) +
                              C715 *
                                  std::exp(-0.00081290825 * std::sqrt(C690) /
                                           (std::pow(C680, 7. / 6.) * C715)) *
                                  0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                                  std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                             C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) /
                             (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                              std::pow(
                                  std::pow(C680, 7. / 3.) *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                  2))) +
                        C778 * C680 *
                            (std::exp(-0.00081290825 * std::sqrt(C690) /
                                      (std::pow(C680, 7. / 6.) * C715)) *
                                 0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                                 std::pow(std::pow(C680, 7. / 6.) * C715, 2) +
                             (std::pow(std::pow(C680, 7. / 6.) * C715, 2) *
                                  (C715 * 0.00081290825 * std::sqrt(C690) *
                                       std::pow(C680, 7. / 6.) *
                                       std::exp(-0.00081290825 * std::sqrt(C690) /
                                                (std::pow(C680, 7. / 6.) * C715)) *
                                       0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                                       std::pow(std::pow(C680, 7. / 6.) * C715, 2) +
                                   std::exp(-0.00081290825 * std::sqrt(C690) /
                                            (std::pow(C680, 7. / 6.) * C715)) *
                                       0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.)) -
                              C715 *
                                  std::exp(-0.00081290825 * std::sqrt(C690) /
                                           (std::pow(C680, 7. / 6.) * C715)) *
                                  0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * 2 *
                                  C715 * std::pow(C680, 14. / 6.)) /
                                 std::pow(std::pow(C680, 7. / 6.) * C715, 4)) *
                            C690 /
                            (std::pow(C680, 7. / 3.) *
                             std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.))) *
                           C778) +
                      ((((2. - (std::pow(C692, C684) + std::pow(C693, C684))) /
                             (2. * (std::pow(2., C682) - 1.)) +
                         1) *
                            C749 -
                        C755 * C680 * C684 * std::pow(C692, C684 - 1) /
                            (2. * (std::pow(2., C682) - 1.))) -
                       C756 * C680 * C684 * std::pow(C693, C684 - 1) /
                           (2. * (std::pow(2., C682) - 1.))) *
                          C779) +
                     C766 *
                         ((C749 * 0.01555 * ((std::pow(C692, C684) + std::pow(C693, C684)) - 2.) +
                           C755 * C680 * 0.01555 * C684 * std::pow(C692, C684 - 1)) +
                          C756 * C680 * 0.01555 * C684 * std::pow(C693, C684 - 1)) /
                         (2. * (std::pow(2., C682) - 1.))) +
                    C768 *
                        ((C749 * 0.0007 * ((std::pow(C692, C684) + std::pow(C693, C684)) - 2.) +
                          C755 * C680 * 0.0007 * C684 * std::pow(C692, C684 - 1)) +
                         C756 * C680 * 0.0007 * C684 * std::pow(C693, C684 - 1)) /
                        (2. * (std::pow(2., C682) - 1.))) +
                   C759 *
                       ((C749 * -0.0048 * ((std::pow(C692, C684) + std::pow(C693, C684)) - 2.) +
                         C755 * C680 * -0.0048 * C684 * std::pow(C692, C684 - 1)) +
                        C756 * C680 * -0.0048 * C684 * std::pow(C693, C684 - 1)) /
                       (2. * (std::pow(2., C682) - 1.))) +
                  C680 *
                      ((2. - (std::pow(C692, C684) + std::pow(C693, C684))) /
                           (2. * (std::pow(2., C682) - 1.)) +
                       1) *
                      C1109) +
                 C1096 * C680 * 0.01555 * ((std::pow(C692, C684) + std::pow(C693, C684)) - 2.) /
                     (2. * (std::pow(2., C682) - 1.))) +
                C1098 * C680 * 0.0007 * ((std::pow(C692, C684) + std::pow(C693, C684)) - 2.) /
                    (2. * (std::pow(2., C682) - 1.))) +
               C1089 * C680 * -0.0048 * ((std::pow(C692, C684) + std::pow(C693, C684)) - 2.) /
                   (2. * (std::pow(2., C682) - 1.))) +
              C680 *
                  (((((0.01555 * C703 + 0.0007 * C705) - 0.0269) - 0.0048 * C696) - C716) * C684 *
                       std::pow(C692, C684 - 1) / (2. * (std::pow(2., C682) - 1.)) -
                   std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                       C715 * C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) /
                       (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                        std::pow(std::pow(C680, 7. / 3.) *
                                     std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                 2))) *
                  C1085) +
             C680 *
                 (((((0.01555 * C703 + 0.0007 * C705) - 0.0269) - 0.0048 * C696) - C716) * C684 *
                      std::pow(C693, C684 - 1) / (2. * (std::pow(2., C682) - 1.)) -
                  std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                      C715 * C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) /
                      (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                       std::pow(std::pow(C680, 7. / 3.) *
                                    std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                2))) *
                 C1086) +
            C1108 * C680 *
                (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                 C715 *
                     std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                     0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                     std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                C690 /
                (std::pow(C680, 7. / 3.) *
                 std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.));

        const double C1110 = -24. * Pi;
        const double C1111 = 0.88668 * Pi;
        const double C1112 = 2. * sigmaAb;
        const double C1113 = 12. * Pi;
        const double C1114 = rhoA + rhoB;
        const double C1115 = rhoA - rhoB;
        const double C1116 = -2. / 3.;
        const double C1117 = -1. / 3.;
        const double C1118 = 1. / 3.;
        const double C1119 = 2. / 3.;
        const double C1120 = 4. / 3.;
        const double C1121 = 5. / 3.;
        const double C1122 = 7. / 6.;
        const double C1123 = -0.9999999999999999 * C1114;
        const double C1124 = 0.9999999999999999 * C1115;
        const double C1125 = Pi * C1114;
        const double C1126 = sigmaAa + C1112;
        const double C1127 = std::pow(C1114, C1120);
        const double C1128 = std::pow(C1114, C1122);
        const double C1129 = 4. * C1125;
        const double C1130 = C1126 + sigmaBb;
        const double C1131 = C1123 - C1124;
        const double C1132 = C1124 / C1114;
        const double C1133 = C1132 + 1.;
        const double C1134 = 1. - C1132;
        const double C1135 = 0.22167 / C1129;
        const double C1136 = 3. / C1129;
        const double C1137 = std::sqrt(C1130);
        const double C1138 = std::pow(C1129, 2);
        const double C1139 = 0.00081290825 * C1137;
        const double C1140 = 3. * C1138;
        const double C1141 = C1111 / C1138;
        const double C1142 = std::pow(C1133, C1121);
        const double C1143 = std::pow(C1134, C1121);
        const double C1144 = std::pow(C1136, C1116);
        const double C1145 = std::pow(C1136, C1117);
        const double C1146 = std::pow(C1136, C1118);
        const double C1147 = std::pow(C1136, C1119);
        const double C1148 = 7.389e-6 * C1147;
        const double C1149 = 0.023266 * C1146;
        const double C1150 = 0.472 * C1147;
        const double C1151 = 8.723 * C1146;
        const double C1152 = C1144 * C1113;
        const double C1153 = C1145 * C1110;
        const double C1154 = C1142 + C1143;
        const double C1155 = -8.723 * C1152;
        const double C1156 = -0.023266 * C1152;
        const double C1157 = 7.389e-6 * C1153;
        const double C1158 = 0.472 * C1153;
        const double C1159 = C1149 + C1148;
        const double C1160 = C1151 + C1150;
        const double C1161 = C1154 / 2.;
        const double C1162 = C1155 + C1158;
        const double C1163 = C1156 + C1157;
        const double C1164 = C1159 + 0.002568;
        const double C1165 = C1160 + C1135;
        const double C1166 = std::sqrt(C1161);
        const double C1167 = C1127 * C1166;
        const double C1168 = C1165 + 1.;
        const double C1169 = C1162 / C1140;
        const double C1170 = C1168 * C1163;
        const double C1171 = C1169 - C1141;
        const double C1172 = C1164 / C1168;
        const double C1173 = std::pow(C1168, 2);
        const double C1174 = C1164 * C1171;
        const double C1175 = C1172 + 0.001667;
        const double C1176 = C1170 / C1140;
        const double C1177 = C1175 * C1128;
        const double C1178 = C1176 - C1174;
        const double C1179 = C1139 / C1177;
        const double C1180 = -C1179;
        const double C1181 = std::exp(C1180);
        matrix.upper[6] =
            C1181 * (1. - C1139 / (2. * C1177)) *
                (C1167 * C1178 / C1173 - C1175 * (C1127 *
                                                      (std::pow(C1133, C1119) * 5. * C1131 +
                                                       std::pow(C1134, C1119) * -5. * C1131) /
                                                      (6. * std::pow(C1114, 2) * 2 * C1166) +
                                                  C1166 * 4. * std::pow(C1114, C1118) / 3.)) /
                std::pow(C1167, 2) +
            C1175 * 0.00081290825 *
                (C1175 * 7. * std::pow(C1114, 1. / 6.) / 6. + C1128 * C1178 / C1173) *
                (3. - C1179) * C1181 * C1137 / (2. * std::pow(C1177, 2) * C1167);

        const double C1182 = -24. * Pi;
        const double C1183 = 0.88668 * Pi;
        const double C1184 = 2. * sigmaAb;
        const double C1185 = 12. * Pi;
        const double C1186 = rhoA + rhoB;
        const double C1187 = rhoA - rhoB;
        const double C1188 = -2. / 3.;
        const double C1189 = -1. / 3.;
        const double C1190 = 1. / 3.;
        const double C1191 = 2. / 3.;
        const double C1192 = 4. / 3.;
        const double C1193 = 5. / 3.;
        const double C1194 = 7. / 6.;
        const double C1195 = -0.9999999999999999 * C1186;
        const double C1196 = 0.9999999999999999 * C1187;
        const double C1197 = Pi * C1186;
        const double C1198 = sigmaAa + C1184;
        const double C1199 = std::pow(C1186, C1192);
        const double C1200 = std::pow(C1186, C1194);
        const double C1201 = 4. * C1197;
        const double C1202 = C1198 + sigmaBb;
        const double C1203 = C1195 - C1196;
        const double C1204 = C1196 / C1186;
        const double C1205 = C1204 + 1.;
        const double C1206 = 1. - C1204;
        const double C1207 = 0.22167 / C1201;
        const double C1208 = 3. / C1201;
        const double C1209 = std::sqrt(C1202);
        const double C1210 = std::pow(C1201, 2);
        const double C1211 = 0.00081290825 * C1209;
        const double C1212 = 3. * C1210;
        const double C1213 = C1183 / C1210;
        const double C1214 = std::pow(C1205, C1193);
        const double C1215 = std::pow(C1206, C1193);
        const double C1216 = std::pow(C1208, C1188);
        const double C1217 = std::pow(C1208, C1189);
        const double C1218 = std::pow(C1208, C1190);
        const double C1219 = std::pow(C1208, C1191);
        const double C1220 = 7.389e-6 * C1219;
        const double C1221 = 0.023266 * C1218;
        const double C1222 = 0.472 * C1219;
        const double C1223 = 8.723 * C1218;
        const double C1224 = C1216 * C1185;
        const double C1225 = C1217 * C1182;
        const double C1226 = C1214 + C1215;
        const double C1227 = -8.723 * C1224;
        const double C1228 = -0.023266 * C1224;
        const double C1229 = 7.389e-6 * C1225;
        const double C1230 = 0.472 * C1225;
        const double C1231 = C1221 + C1220;
        const double C1232 = C1223 + C1222;
        const double C1233 = C1226 / 2.;
        const double C1234 = C1227 + C1230;
        const double C1235 = C1228 + C1229;
        const double C1236 = C1231 + 0.002568;
        const double C1237 = C1232 + C1207;
        const double C1238 = std::sqrt(C1233);
        const double C1239 = C1199 * C1238;
        const double C1240 = C1237 + 1.;
        const double C1241 = C1234 / C1212;
        const double C1242 = C1240 * C1235;
        const double C1243 = C1241 - C1213;
        const double C1244 = C1236 / C1240;
        const double C1245 = std::pow(C1240, 2);
        const double C1246 = C1236 * C1243;
        const double C1247 = C1244 + 0.001667;
        const double C1248 = C1242 / C1212;
        const double C1249 = C1247 * C1200;
        const double C1250 = C1248 - C1246;
        const double C1251 = C1211 / C1249;
        const double C1252 = -C1251;
        const double C1253 = std::exp(C1252);
        matrix.upper[7] =
            2. * (C1253 * (1. - C1211 / (2. * C1249)) *
                      (C1239 * C1250 / C1245 - C1247 * (C1199 *
                                                            (std::pow(C1205, C1191) * 5. * C1203 +
                                                             std::pow(C1206, C1191) * -5. * C1203) /
                                                            (6. * std::pow(C1186, 2) * 2 * C1238) +
                                                        C1238 * 4. * std::pow(C1186, C1190) / 3.)) /
                      std::pow(C1239, 2) +
                  C1247 * 0.00081290825 *
                      (C1247 * 7. * std::pow(C1186, 1. / 6.) / 6. + C1200 * C1250 / C1245) *
                      (3. - C1251) * C1253 * C1209 / (2. * std::pow(C1249, 2) * C1239));

        const double C1254 = -24. * Pi;
        const double C1255 = 0.88668 * Pi;
        const double C1256 = 2. * sigmaAb;
        const double C1257 = 12. * Pi;
        const double C1258 = rhoA + rhoB;
        const double C1259 = rhoA - rhoB;
        const double C1260 = -2. / 3.;
        const double C1261 = -1. / 3.;
        const double C1262 = 1. / 3.;
        const double C1263 = 2. / 3.;
        const double C1264 = 4. / 3.;
        const double C1265 = 5. / 3.;
        const double C1266 = 7. / 6.;
        const double C1267 = -0.9999999999999999 * C1258;
        const double C1268 = 0.9999999999999999 * C1259;
        const double C1269 = Pi * C1258;
        const double C1270 = sigmaAa + C1256;
        const double C1271 = std::pow(C1258, C1264);
        const double C1272 = std::pow(C1258, C1266);
        const double C1273 = 4. * C1269;
        const double C1274 = C1270 + sigmaBb;
        const double C1275 = C1267 - C1268;
        const double C1276 = C1268 / C1258;
        const double C1277 = C1276 + 1.;
        const double C1278 = 1. - C1276;
        const double C1279 = 0.22167 / C1273;
        const double C1280 = 3. / C1273;
        const double C1281 = std::sqrt(C1274);
        const double C1282 = std::pow(C1273, 2);
        const double C1283 = 0.00081290825 * C1281;
        const double C1284 = 3. * C1282;
        const double C1285 = C1255 / C1282;
        const double C1286 = std::pow(C1277, C1265);
        const double C1287 = std::pow(C1278, C1265);
        const double C1288 = std::pow(C1280, C1260);
        const double C1289 = std::pow(C1280, C1261);
        const double C1290 = std::pow(C1280, C1262);
        const double C1291 = std::pow(C1280, C1263);
        const double C1292 = 7.389e-6 * C1291;
        const double C1293 = 0.023266 * C1290;
        const double C1294 = 0.472 * C1291;
        const double C1295 = 8.723 * C1290;
        const double C1296 = C1288 * C1257;
        const double C1297 = C1289 * C1254;
        const double C1298 = C1286 + C1287;
        const double C1299 = -8.723 * C1296;
        const double C1300 = -0.023266 * C1296;
        const double C1301 = 7.389e-6 * C1297;
        const double C1302 = 0.472 * C1297;
        const double C1303 = C1293 + C1292;
        const double C1304 = C1295 + C1294;
        const double C1305 = C1298 / 2.;
        const double C1306 = C1299 + C1302;
        const double C1307 = C1300 + C1301;
        const double C1308 = C1303 + 0.002568;
        const double C1309 = C1304 + C1279;
        const double C1310 = std::sqrt(C1305);
        const double C1311 = C1271 * C1310;
        const double C1312 = C1309 + 1.;
        const double C1313 = C1306 / C1284;
        const double C1314 = C1312 * C1307;
        const double C1315 = C1313 - C1285;
        const double C1316 = C1308 / C1312;
        const double C1317 = std::pow(C1312, 2);
        const double C1318 = C1308 * C1315;
        const double C1319 = C1316 + 0.001667;
        const double C1320 = C1314 / C1284;
        const double C1321 = C1319 * C1272;
        const double C1322 = C1320 - C1318;
        const double C1323 = C1283 / C1321;
        const double C1324 = -C1323;
        const double C1325 = std::exp(C1324);
        matrix.upper[8] =
            C1325 * (1. - C1283 / (2. * C1321)) *
                (C1311 * C1322 / C1317 - C1319 * (C1271 *
                                                      (std::pow(C1277, C1263) * 5. * C1275 +
                                                       std::pow(C1278, C1263) * -5. * C1275) /
                                                      (6. * std::pow(C1258, 2) * 2 * C1310) +
                                                  C1310 * 4. * std::pow(C1258, C1262) / 3.)) /
                std::pow(C1311, 2) +
            C1319 * 0.00081290825 *
                (C1319 * 7. * std::pow(C1258, 1. / 6.) / 6. + C1272 * C1322 / C1317) *
                (3. - C1323) * C1325 * C1281 / (2. * std::pow(C1321, 2) * C1311);

        const double C780 = 0.;
        const double C781 = 1;
        const double C782 = C781;
        const double C783 = C780 / C680;
        const double C784 = C783;
        const double C785 = -C783;
        const double C786 = 0.;
        const double C787 = C786;
        const double C788 = C787;
        const double C789 = C788 / C712;
        const double C790 = C789;
        matrix.upper[9] =
            (((((C680 *
                     (((((0.01555 * C703 + 0.0007 * C705) - 0.0269) - 0.0048 * C696) - C716) *
                          C684 * (C684 - 1) * std::pow(C692, C684 - 2) /
                          (2. * (std::pow(2., C682) - 1.)) -
                      (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                           std::pow(
                               std::pow(C680, 7. / 3.) *
                                   std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                               2) *
                           std::exp(-0.00081290825 * std::sqrt(C690) /
                                    (std::pow(C680, 7. / 6.) * C715)) *
                           C715 * C690 * std::pow(C680, 7. / 3.) * C685 * (C685 - 1) *
                           std::pow(C692, C685 - 2) -
                       std::exp(-0.00081290825 * std::sqrt(C690) /
                                (std::pow(C680, 7. / 6.) * C715)) *
                           C715 * C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) *
                           4. *
                           (std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                std::pow(C680, 7. / 3.) *
                                std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) * 2 *
                                std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) +
                            std::pow(
                                std::pow(C680, 7. / 3.) *
                                    std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                2) *
                                C685 * std::pow(C692, C685 - 1)) /
                           (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.))) /
                          std::pow(
                              4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                  std::pow(
                                      std::pow(C680, 7. / 3.) *
                                          std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                    2.),
                                      2),
                              2)) *
                     C784 +
                 C785 * C680 *
                     std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                     C715 * C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) * 4. *
                     (std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                          std::pow(C680, 7. / 3.) *
                          std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) * 2 *
                          std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) +
                      std::pow(std::pow(C680, 7. / 3.) *
                                   std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                               2) *
                          C685 * std::pow(C692, C685 - 1)) /
                     (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                      std::pow(
                          4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                              std::pow(
                                  std::pow(C680, 7. / 3.) *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                  2),
                          2))) -
                C782 * C680 *
                    (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                         C715 +
                     C690 * C715 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) /
                         (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715)) *
                    std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) /
                    (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                     std::pow(std::pow(C680, 7. / 3.) *
                                  std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                              2))) -
               C790 * C680 *
                   (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                    C715 *
                        std::exp(-0.00081290825 * std::sqrt(C690) /
                                 (std::pow(C680, 7. / 6.) * C715)) *
                        0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                        std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                   C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) /
                   (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                    std::pow(std::pow(C680, 7. / 3.) *
                                 std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                             2))) *
                  C784 +
              ((C782 * C680 *
                    (C715 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) /
                         (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715) +
                     (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715 *
                          (C690 * C715 * 0.0000006608198229 *
                               std::exp(-0.00081290825 * std::sqrt(C690) /
                                        (std::pow(C680, 7. / 6.) * C715)) /
                               (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715) +
                           C715 * -0.00081290825 *
                               std::exp(-0.00081290825 * std::sqrt(C690) /
                                        (std::pow(C680, 7. / 6.) * C715))) -
                      C690 * C715 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C690) /
                                   (std::pow(C680, 7. / 6.) * C715)) *
                          std::pow(C680, 7. / 6.) * C715 / std::sqrt(C690)) /
                         std::pow(2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715, 2)) /
                    (std::pow(C680, 7. / 3.) *
                     std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.)) -
                (C785 * C680 *
                     (std::exp(-0.00081290825 * std::sqrt(C690) /
                               (std::pow(C680, 7. / 6.) * C715)) *
                          C715 +
                      C690 * C715 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C690) /
                                   (std::pow(C680, 7. / 6.) * C715)) /
                          (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715)) *
                     std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) +
                 C784 * C680 *
                     (std::exp(-0.00081290825 * std::sqrt(C690) /
                               (std::pow(C680, 7. / 6.) * C715)) *
                          C715 +
                      C690 * C715 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C690) /
                                   (std::pow(C680, 7. / 6.) * C715)) /
                          (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715)) *
                     std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1)) /
                    (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                     std::pow(std::pow(C680, 7. / 3.) *
                                  std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                              2))) +
               C790 * C680 *
                   ((std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                     C715 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                         0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                         std::pow(std::pow(C680, 7. / 6.) * C715, 2)) +
                    (-0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) /
                         (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715) +
                     C715 *
                         (std::exp(-0.00081290825 * std::sqrt(C690) /
                                   (std::pow(C680, 7. / 6.) * C715)) *
                              0.00081290825 * std::pow(C680, 7. / 6.) / (2 * std::sqrt(C690)) +
                          0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) *
                              -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C690) /
                                       (std::pow(C680, 7. / 6.) * C715)) /
                              (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715)) /
                         std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                        C690) /
                   (std::pow(C680, 7. / 3.) *
                    std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.))) *
                  C782) +
             ((C782 * C680 *
                   ((std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                     C715 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                         0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                         std::pow(std::pow(C680, 7. / 6.) * C715, 2)) +
                    (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715 * C690 *
                         (C715 * -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C690) /
                                       (std::pow(C680, 7. / 6.) * C715)) *
                              0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                              std::pow(std::pow(C680, 7. / 6.) * C715, 2) -
                          0.00081290825 * std::exp(-0.00081290825 * std::sqrt(C690) /
                                                   (std::pow(C680, 7. / 6.) * C715))) -
                     C690 * C715 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                         2 * std::sqrt(C690) * std::pow(C680, 7. / 6.)) /
                        std::pow(2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715, 2)) /
                   (std::pow(C680, 7. / 3.) *
                    std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.)) -
               (C785 * C680 *
                    (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                     C715 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                         0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                         std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                    C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) +
                C784 * C680 *
                    (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                     C715 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                         0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                         std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                    C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1)) /
                   (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                    std::pow(std::pow(C680, 7. / 3.) *
                                 std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                             2))) +
              C790 * C680 *
                  (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                       0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                       std::pow(std::pow(C680, 7. / 6.) * C715, 2) +
                   (std::pow(std::pow(C680, 7. / 6.) * C715, 2) *
                        (C715 * 0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) *
                             std::exp(-0.00081290825 * std::sqrt(C690) /
                                      (std::pow(C680, 7. / 6.) * C715)) *
                             0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                             std::pow(std::pow(C680, 7. / 6.) * C715, 2) +
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                             0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.)) -
                    C715 *
                        std::exp(-0.00081290825 * std::sqrt(C690) /
                                 (std::pow(C680, 7. / 6.) * C715)) *
                        0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * 2 * C715 *
                        std::pow(C680, 14. / 6.)) /
                       std::pow(std::pow(C680, 7. / 6.) * C715, 4)) *
                  C690 /
                  (std::pow(C680, 7. / 3.) *
                   std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.))) *
                 C790) +
            (((C784 * C680 *
                   std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                   C715 * C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) * 4. *
                   (std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                        std::pow(C680, 7. / 3.) *
                        std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) * 2 *
                        std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) +
                    std::pow(std::pow(C680, 7. / 3.) *
                                 std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                             2) *
                        C685 * std::pow(C693, C685 - 1)) /
                   (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                    std::pow(
                        4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                            std::pow(
                                std::pow(C680, 7. / 3.) *
                                    std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                2),
                        2)) +
               C680 *
                   (((((0.01555 * C703 + 0.0007 * C705) - 0.0269) - 0.0048 * C696) - C716) * C684 *
                        (C684 - 1) * std::pow(C693, C684 - 2) / (2. * (std::pow(2., C682) - 1.)) -
                    (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                         std::pow(std::pow(C680, 7. / 3.) *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                  2) *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                         C715 * C690 * std::pow(C680, 7. / 3.) * C685 * (C685 - 1) *
                         std::pow(C693, C685 - 2) -
                     std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                         C715 * C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) *
                         4. *
                         (std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                              std::pow(C680, 7. / 3.) *
                              std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) * 2 *
                              std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) +
                          std::pow(
                              std::pow(C680, 7. / 3.) *
                                  std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                              2) *
                              C685 * std::pow(C693, C685 - 1)) /
                         (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.))) /
                        std::pow(
                            4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                std::pow(
                                    std::pow(C680, 7. / 3.) *
                                        std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                  2.),
                                    2),
                            2)) *
                   C785) -
              C782 * C680 *
                  (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                       C715 +
                   C690 * C715 * -0.00081290825 *
                       std::exp(-0.00081290825 * std::sqrt(C690) /
                                (std::pow(C680, 7. / 6.) * C715)) /
                       (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715)) *
                  std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) /
                  (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                   std::pow(std::pow(C680, 7. / 3.) *
                                std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                            2))) -
             C790 * C680 *
                 (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                  C715 *
                      std::exp(-0.00081290825 * std::sqrt(C690) /
                               (std::pow(C680, 7. / 6.) * C715)) *
                      0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                      std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                 C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) /
                 (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                  std::pow(std::pow(C680, 7. / 3.) *
                               std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                           2))) *
                C785;

        const double C791 = 2.;
        const double C792 = 0.;
        const double C793 = C791;
        const double C794 = C793;
        const double C795 = C792 / C680;
        const double C796 = C795;
        const double C797 = -C795;
        const double C798 = 0.;
        const double C799 = C798;
        const double C800 = C799;
        const double C801 = C800 / C712;
        const double C802 = C801;
        matrix.upper[10] =
            (((((C680 *
                     (((((0.01555 * C703 + 0.0007 * C705) - 0.0269) - 0.0048 * C696) - C716) *
                          C684 * (C684 - 1) * std::pow(C692, C684 - 2) /
                          (2. * (std::pow(2., C682) - 1.)) -
                      (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                           std::pow(
                               std::pow(C680, 7. / 3.) *
                                   std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                               2) *
                           std::exp(-0.00081290825 * std::sqrt(C690) /
                                    (std::pow(C680, 7. / 6.) * C715)) *
                           C715 * C690 * std::pow(C680, 7. / 3.) * C685 * (C685 - 1) *
                           std::pow(C692, C685 - 2) -
                       std::exp(-0.00081290825 * std::sqrt(C690) /
                                (std::pow(C680, 7. / 6.) * C715)) *
                           C715 * C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) *
                           4. *
                           (std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                std::pow(C680, 7. / 3.) *
                                std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) * 2 *
                                std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) +
                            std::pow(
                                std::pow(C680, 7. / 3.) *
                                    std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                2) *
                                C685 * std::pow(C692, C685 - 1)) /
                           (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.))) /
                          std::pow(
                              4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                  std::pow(
                                      std::pow(C680, 7. / 3.) *
                                          std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                    2.),
                                      2),
                              2)) *
                     C784 +
                 C785 * C680 *
                     std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                     C715 * C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) * 4. *
                     (std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                          std::pow(C680, 7. / 3.) *
                          std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) * 2 *
                          std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) +
                      std::pow(std::pow(C680, 7. / 3.) *
                                   std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                               2) *
                          C685 * std::pow(C692, C685 - 1)) /
                     (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                      std::pow(
                          4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                              std::pow(
                                  std::pow(C680, 7. / 3.) *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                  2),
                          2))) -
                C782 * C680 *
                    (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                         C715 +
                     C690 * C715 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) /
                         (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715)) *
                    std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) /
                    (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                     std::pow(std::pow(C680, 7. / 3.) *
                                  std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                              2))) -
               C790 * C680 *
                   (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                    C715 *
                        std::exp(-0.00081290825 * std::sqrt(C690) /
                                 (std::pow(C680, 7. / 6.) * C715)) *
                        0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                        std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                   C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) /
                   (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                    std::pow(std::pow(C680, 7. / 3.) *
                                 std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                             2))) *
                  C796 +
              ((C782 * C680 *
                    (C715 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) /
                         (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715) +
                     (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715 *
                          (C690 * C715 * 0.0000006608198229 *
                               std::exp(-0.00081290825 * std::sqrt(C690) /
                                        (std::pow(C680, 7. / 6.) * C715)) /
                               (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715) +
                           C715 * -0.00081290825 *
                               std::exp(-0.00081290825 * std::sqrt(C690) /
                                        (std::pow(C680, 7. / 6.) * C715))) -
                      C690 * C715 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C690) /
                                   (std::pow(C680, 7. / 6.) * C715)) *
                          std::pow(C680, 7. / 6.) * C715 / std::sqrt(C690)) /
                         std::pow(2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715, 2)) /
                    (std::pow(C680, 7. / 3.) *
                     std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.)) -
                (C785 * C680 *
                     (std::exp(-0.00081290825 * std::sqrt(C690) /
                               (std::pow(C680, 7. / 6.) * C715)) *
                          C715 +
                      C690 * C715 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C690) /
                                   (std::pow(C680, 7. / 6.) * C715)) /
                          (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715)) *
                     std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) +
                 C784 * C680 *
                     (std::exp(-0.00081290825 * std::sqrt(C690) /
                               (std::pow(C680, 7. / 6.) * C715)) *
                          C715 +
                      C690 * C715 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C690) /
                                   (std::pow(C680, 7. / 6.) * C715)) /
                          (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715)) *
                     std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1)) /
                    (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                     std::pow(std::pow(C680, 7. / 3.) *
                                  std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                              2))) +
               C790 * C680 *
                   ((std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                     C715 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                         0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                         std::pow(std::pow(C680, 7. / 6.) * C715, 2)) +
                    (-0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) /
                         (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715) +
                     C715 *
                         (std::exp(-0.00081290825 * std::sqrt(C690) /
                                   (std::pow(C680, 7. / 6.) * C715)) *
                              0.00081290825 * std::pow(C680, 7. / 6.) / (2 * std::sqrt(C690)) +
                          0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) *
                              -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C690) /
                                       (std::pow(C680, 7. / 6.) * C715)) /
                              (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715)) /
                         std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                        C690) /
                   (std::pow(C680, 7. / 3.) *
                    std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.))) *
                  C794) +
             ((C782 * C680 *
                   ((std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                     C715 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                         0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                         std::pow(std::pow(C680, 7. / 6.) * C715, 2)) +
                    (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715 * C690 *
                         (C715 * -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C690) /
                                       (std::pow(C680, 7. / 6.) * C715)) *
                              0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                              std::pow(std::pow(C680, 7. / 6.) * C715, 2) -
                          0.00081290825 * std::exp(-0.00081290825 * std::sqrt(C690) /
                                                   (std::pow(C680, 7. / 6.) * C715))) -
                     C690 * C715 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                         2 * std::sqrt(C690) * std::pow(C680, 7. / 6.)) /
                        std::pow(2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715, 2)) /
                   (std::pow(C680, 7. / 3.) *
                    std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.)) -
               (C785 * C680 *
                    (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                     C715 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                         0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                         std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                    C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) +
                C784 * C680 *
                    (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                     C715 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                         0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                         std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                    C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1)) /
                   (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                    std::pow(std::pow(C680, 7. / 3.) *
                                 std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                             2))) +
              C790 * C680 *
                  (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                       0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                       std::pow(std::pow(C680, 7. / 6.) * C715, 2) +
                   (std::pow(std::pow(C680, 7. / 6.) * C715, 2) *
                        (C715 * 0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) *
                             std::exp(-0.00081290825 * std::sqrt(C690) /
                                      (std::pow(C680, 7. / 6.) * C715)) *
                             0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                             std::pow(std::pow(C680, 7. / 6.) * C715, 2) +
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                             0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.)) -
                    C715 *
                        std::exp(-0.00081290825 * std::sqrt(C690) /
                                 (std::pow(C680, 7. / 6.) * C715)) *
                        0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * 2 * C715 *
                        std::pow(C680, 14. / 6.)) /
                       std::pow(std::pow(C680, 7. / 6.) * C715, 4)) *
                  C690 /
                  (std::pow(C680, 7. / 3.) *
                   std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.))) *
                 C802) +
            (((C784 * C680 *
                   std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                   C715 * C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) * 4. *
                   (std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                        std::pow(C680, 7. / 3.) *
                        std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) * 2 *
                        std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) +
                    std::pow(std::pow(C680, 7. / 3.) *
                                 std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                             2) *
                        C685 * std::pow(C693, C685 - 1)) /
                   (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                    std::pow(
                        4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                            std::pow(
                                std::pow(C680, 7. / 3.) *
                                    std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                2),
                        2)) +
               C680 *
                   (((((0.01555 * C703 + 0.0007 * C705) - 0.0269) - 0.0048 * C696) - C716) * C684 *
                        (C684 - 1) * std::pow(C693, C684 - 2) / (2. * (std::pow(2., C682) - 1.)) -
                    (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                         std::pow(std::pow(C680, 7. / 3.) *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                  2) *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                         C715 * C690 * std::pow(C680, 7. / 3.) * C685 * (C685 - 1) *
                         std::pow(C693, C685 - 2) -
                     std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                         C715 * C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) *
                         4. *
                         (std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                              std::pow(C680, 7. / 3.) *
                              std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) * 2 *
                              std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) +
                          std::pow(
                              std::pow(C680, 7. / 3.) *
                                  std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                              2) *
                              C685 * std::pow(C693, C685 - 1)) /
                         (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.))) /
                        std::pow(
                            4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                std::pow(
                                    std::pow(C680, 7. / 3.) *
                                        std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                  2.),
                                    2),
                            2)) *
                   C785) -
              C782 * C680 *
                  (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                       C715 +
                   C690 * C715 * -0.00081290825 *
                       std::exp(-0.00081290825 * std::sqrt(C690) /
                                (std::pow(C680, 7. / 6.) * C715)) /
                       (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715)) *
                  std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) /
                  (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                   std::pow(std::pow(C680, 7. / 3.) *
                                std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                            2))) -
             C790 * C680 *
                 (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                  C715 *
                      std::exp(-0.00081290825 * std::sqrt(C690) /
                               (std::pow(C680, 7. / 6.) * C715)) *
                      0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                      std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                 C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) /
                 (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                  std::pow(std::pow(C680, 7. / 3.) *
                               std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                           2))) *
                C797;

        const double C803 = 0.;
        const double C804 = 1;
        const double C805 = C803 / C680;
        const double C806 = C805;
        const double C807 = -C805;
        const double C808 = 0.;
        const double C809 = C808;
        const double C810 = C809;
        const double C811 = C810 / C712;
        const double C812 = C811;
        matrix.upper[11] =
            (((((C680 *
                     (((((0.01555 * C703 + 0.0007 * C705) - 0.0269) - 0.0048 * C696) - C716) *
                          C684 * (C684 - 1) * std::pow(C692, C684 - 2) /
                          (2. * (std::pow(2., C682) - 1.)) -
                      (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                           std::pow(
                               std::pow(C680, 7. / 3.) *
                                   std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                               2) *
                           std::exp(-0.00081290825 * std::sqrt(C690) /
                                    (std::pow(C680, 7. / 6.) * C715)) *
                           C715 * C690 * std::pow(C680, 7. / 3.) * C685 * (C685 - 1) *
                           std::pow(C692, C685 - 2) -
                       std::exp(-0.00081290825 * std::sqrt(C690) /
                                (std::pow(C680, 7. / 6.) * C715)) *
                           C715 * C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) *
                           4. *
                           (std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                std::pow(C680, 7. / 3.) *
                                std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) * 2 *
                                std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) +
                            std::pow(
                                std::pow(C680, 7. / 3.) *
                                    std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                2) *
                                C685 * std::pow(C692, C685 - 1)) /
                           (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.))) /
                          std::pow(
                              4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                  std::pow(
                                      std::pow(C680, 7. / 3.) *
                                          std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                    2.),
                                      2),
                              2)) *
                     C784 +
                 C785 * C680 *
                     std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                     C715 * C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) * 4. *
                     (std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                          std::pow(C680, 7. / 3.) *
                          std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) * 2 *
                          std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) +
                      std::pow(std::pow(C680, 7. / 3.) *
                                   std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                               2) *
                          C685 * std::pow(C692, C685 - 1)) /
                     (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                      std::pow(
                          4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                              std::pow(
                                  std::pow(C680, 7. / 3.) *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                  2),
                          2))) -
                C782 * C680 *
                    (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                         C715 +
                     C690 * C715 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) /
                         (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715)) *
                    std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) /
                    (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                     std::pow(std::pow(C680, 7. / 3.) *
                                  std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                              2))) -
               C790 * C680 *
                   (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                    C715 *
                        std::exp(-0.00081290825 * std::sqrt(C690) /
                                 (std::pow(C680, 7. / 6.) * C715)) *
                        0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                        std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                   C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) /
                   (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                    std::pow(std::pow(C680, 7. / 3.) *
                                 std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                             2))) *
                  C806 +
              ((C782 * C680 *
                    (C715 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) /
                         (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715) +
                     (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715 *
                          (C690 * C715 * 0.0000006608198229 *
                               std::exp(-0.00081290825 * std::sqrt(C690) /
                                        (std::pow(C680, 7. / 6.) * C715)) /
                               (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715) +
                           C715 * -0.00081290825 *
                               std::exp(-0.00081290825 * std::sqrt(C690) /
                                        (std::pow(C680, 7. / 6.) * C715))) -
                      C690 * C715 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C690) /
                                   (std::pow(C680, 7. / 6.) * C715)) *
                          std::pow(C680, 7. / 6.) * C715 / std::sqrt(C690)) /
                         std::pow(2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715, 2)) /
                    (std::pow(C680, 7. / 3.) *
                     std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.)) -
                (C785 * C680 *
                     (std::exp(-0.00081290825 * std::sqrt(C690) /
                               (std::pow(C680, 7. / 6.) * C715)) *
                          C715 +
                      C690 * C715 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C690) /
                                   (std::pow(C680, 7. / 6.) * C715)) /
                          (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715)) *
                     std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) +
                 C784 * C680 *
                     (std::exp(-0.00081290825 * std::sqrt(C690) /
                               (std::pow(C680, 7. / 6.) * C715)) *
                          C715 +
                      C690 * C715 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C690) /
                                   (std::pow(C680, 7. / 6.) * C715)) /
                          (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715)) *
                     std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1)) /
                    (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                     std::pow(std::pow(C680, 7. / 3.) *
                                  std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                              2))) +
               C790 * C680 *
                   ((std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                     C715 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                         0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                         std::pow(std::pow(C680, 7. / 6.) * C715, 2)) +
                    (-0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) /
                         (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715) +
                     C715 *
                         (std::exp(-0.00081290825 * std::sqrt(C690) /
                                   (std::pow(C680, 7. / 6.) * C715)) *
                              0.00081290825 * std::pow(C680, 7. / 6.) / (2 * std::sqrt(C690)) +
                          0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) *
                              -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C690) /
                                       (std::pow(C680, 7. / 6.) * C715)) /
                              (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715)) /
                         std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                        C690) /
                   (std::pow(C680, 7. / 3.) *
                    std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.))) *
                  C804) +
             ((C782 * C680 *
                   ((std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                     C715 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                         0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                         std::pow(std::pow(C680, 7. / 6.) * C715, 2)) +
                    (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715 * C690 *
                         (C715 * -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C690) /
                                       (std::pow(C680, 7. / 6.) * C715)) *
                              0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                              std::pow(std::pow(C680, 7. / 6.) * C715, 2) -
                          0.00081290825 * std::exp(-0.00081290825 * std::sqrt(C690) /
                                                   (std::pow(C680, 7. / 6.) * C715))) -
                     C690 * C715 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                         2 * std::sqrt(C690) * std::pow(C680, 7. / 6.)) /
                        std::pow(2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715, 2)) /
                   (std::pow(C680, 7. / 3.) *
                    std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.)) -
               (C785 * C680 *
                    (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                     C715 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                         0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                         std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                    C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) +
                C784 * C680 *
                    (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                     C715 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                         0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                         std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                    C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1)) /
                   (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                    std::pow(std::pow(C680, 7. / 3.) *
                                 std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                             2))) +
              C790 * C680 *
                  (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                       0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                       std::pow(std::pow(C680, 7. / 6.) * C715, 2) +
                   (std::pow(std::pow(C680, 7. / 6.) * C715, 2) *
                        (C715 * 0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) *
                             std::exp(-0.00081290825 * std::sqrt(C690) /
                                      (std::pow(C680, 7. / 6.) * C715)) *
                             0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                             std::pow(std::pow(C680, 7. / 6.) * C715, 2) +
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                             0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.)) -
                    C715 *
                        std::exp(-0.00081290825 * std::sqrt(C690) /
                                 (std::pow(C680, 7. / 6.) * C715)) *
                        0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * 2 * C715 *
                        std::pow(C680, 14. / 6.)) /
                       std::pow(std::pow(C680, 7. / 6.) * C715, 4)) *
                  C690 /
                  (std::pow(C680, 7. / 3.) *
                   std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.))) *
                 C812) +
            (((C784 * C680 *
                   std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                   C715 * C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) * 4. *
                   (std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                        std::pow(C680, 7. / 3.) *
                        std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) * 2 *
                        std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) +
                    std::pow(std::pow(C680, 7. / 3.) *
                                 std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                             2) *
                        C685 * std::pow(C693, C685 - 1)) /
                   (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                    std::pow(
                        4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                            std::pow(
                                std::pow(C680, 7. / 3.) *
                                    std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                2),
                        2)) +
               C680 *
                   (((((0.01555 * C703 + 0.0007 * C705) - 0.0269) - 0.0048 * C696) - C716) * C684 *
                        (C684 - 1) * std::pow(C693, C684 - 2) / (2. * (std::pow(2., C682) - 1.)) -
                    (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                         std::pow(std::pow(C680, 7. / 3.) *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                  2) *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                         C715 * C690 * std::pow(C680, 7. / 3.) * C685 * (C685 - 1) *
                         std::pow(C693, C685 - 2) -
                     std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                         C715 * C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) *
                         4. *
                         (std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                              std::pow(C680, 7. / 3.) *
                              std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) * 2 *
                              std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) +
                          std::pow(
                              std::pow(C680, 7. / 3.) *
                                  std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                              2) *
                              C685 * std::pow(C693, C685 - 1)) /
                         (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.))) /
                        std::pow(
                            4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                std::pow(
                                    std::pow(C680, 7. / 3.) *
                                        std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                  2.),
                                    2),
                            2)) *
                   C785) -
              C782 * C680 *
                  (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                       C715 +
                   C690 * C715 * -0.00081290825 *
                       std::exp(-0.00081290825 * std::sqrt(C690) /
                                (std::pow(C680, 7. / 6.) * C715)) /
                       (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715)) *
                  std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) /
                  (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                   std::pow(std::pow(C680, 7. / 3.) *
                                std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                            2))) -
             C790 * C680 *
                 (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                  C715 *
                      std::exp(-0.00081290825 * std::sqrt(C690) /
                               (std::pow(C680, 7. / 6.) * C715)) *
                      0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                      std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                 C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) /
                 (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                  std::pow(std::pow(C680, 7. / 3.) *
                               std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                           2))) *
                C807;

        matrix.upper[12] =
            (((((C680 *
                     (((((0.01555 * C703 + 0.0007 * C705) - 0.0269) - 0.0048 * C696) - C716) *
                          C684 * (C684 - 1) * std::pow(C692, C684 - 2) /
                          (2. * (std::pow(2., C682) - 1.)) -
                      (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                           std::pow(
                               std::pow(C680, 7. / 3.) *
                                   std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                               2) *
                           std::exp(-0.00081290825 * std::sqrt(C690) /
                                    (std::pow(C680, 7. / 6.) * C715)) *
                           C715 * C690 * std::pow(C680, 7. / 3.) * C685 * (C685 - 1) *
                           std::pow(C692, C685 - 2) -
                       std::exp(-0.00081290825 * std::sqrt(C690) /
                                (std::pow(C680, 7. / 6.) * C715)) *
                           C715 * C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) *
                           4. *
                           (std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                std::pow(C680, 7. / 3.) *
                                std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) * 2 *
                                std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) +
                            std::pow(
                                std::pow(C680, 7. / 3.) *
                                    std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                2) *
                                C685 * std::pow(C692, C685 - 1)) /
                           (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.))) /
                          std::pow(
                              4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                  std::pow(
                                      std::pow(C680, 7. / 3.) *
                                          std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                    2.),
                                      2),
                              2)) *
                     C796 +
                 C797 * C680 *
                     std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                     C715 * C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) * 4. *
                     (std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                          std::pow(C680, 7. / 3.) *
                          std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) * 2 *
                          std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) +
                      std::pow(std::pow(C680, 7. / 3.) *
                                   std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                               2) *
                          C685 * std::pow(C692, C685 - 1)) /
                     (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                      std::pow(
                          4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                              std::pow(
                                  std::pow(C680, 7. / 3.) *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                  2),
                          2))) -
                C794 * C680 *
                    (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                         C715 +
                     C690 * C715 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) /
                         (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715)) *
                    std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) /
                    (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                     std::pow(std::pow(C680, 7. / 3.) *
                                  std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                              2))) -
               C802 * C680 *
                   (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                    C715 *
                        std::exp(-0.00081290825 * std::sqrt(C690) /
                                 (std::pow(C680, 7. / 6.) * C715)) *
                        0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                        std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                   C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) /
                   (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                    std::pow(std::pow(C680, 7. / 3.) *
                                 std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                             2))) *
                  C796 +
              ((C794 * C680 *
                    (C715 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) /
                         (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715) +
                     (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715 *
                          (C690 * C715 * 0.0000006608198229 *
                               std::exp(-0.00081290825 * std::sqrt(C690) /
                                        (std::pow(C680, 7. / 6.) * C715)) /
                               (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715) +
                           C715 * -0.00081290825 *
                               std::exp(-0.00081290825 * std::sqrt(C690) /
                                        (std::pow(C680, 7. / 6.) * C715))) -
                      C690 * C715 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C690) /
                                   (std::pow(C680, 7. / 6.) * C715)) *
                          std::pow(C680, 7. / 6.) * C715 / std::sqrt(C690)) /
                         std::pow(2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715, 2)) /
                    (std::pow(C680, 7. / 3.) *
                     std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.)) -
                (C797 * C680 *
                     (std::exp(-0.00081290825 * std::sqrt(C690) /
                               (std::pow(C680, 7. / 6.) * C715)) *
                          C715 +
                      C690 * C715 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C690) /
                                   (std::pow(C680, 7. / 6.) * C715)) /
                          (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715)) *
                     std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) +
                 C796 * C680 *
                     (std::exp(-0.00081290825 * std::sqrt(C690) /
                               (std::pow(C680, 7. / 6.) * C715)) *
                          C715 +
                      C690 * C715 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C690) /
                                   (std::pow(C680, 7. / 6.) * C715)) /
                          (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715)) *
                     std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1)) /
                    (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                     std::pow(std::pow(C680, 7. / 3.) *
                                  std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                              2))) +
               C802 * C680 *
                   ((std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                     C715 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                         0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                         std::pow(std::pow(C680, 7. / 6.) * C715, 2)) +
                    (-0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) /
                         (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715) +
                     C715 *
                         (std::exp(-0.00081290825 * std::sqrt(C690) /
                                   (std::pow(C680, 7. / 6.) * C715)) *
                              0.00081290825 * std::pow(C680, 7. / 6.) / (2 * std::sqrt(C690)) +
                          0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) *
                              -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C690) /
                                       (std::pow(C680, 7. / 6.) * C715)) /
                              (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715)) /
                         std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                        C690) /
                   (std::pow(C680, 7. / 3.) *
                    std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.))) *
                  C794) +
             ((C794 * C680 *
                   ((std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                     C715 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                         0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                         std::pow(std::pow(C680, 7. / 6.) * C715, 2)) +
                    (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715 * C690 *
                         (C715 * -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C690) /
                                       (std::pow(C680, 7. / 6.) * C715)) *
                              0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                              std::pow(std::pow(C680, 7. / 6.) * C715, 2) -
                          0.00081290825 * std::exp(-0.00081290825 * std::sqrt(C690) /
                                                   (std::pow(C680, 7. / 6.) * C715))) -
                     C690 * C715 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                         2 * std::sqrt(C690) * std::pow(C680, 7. / 6.)) /
                        std::pow(2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715, 2)) /
                   (std::pow(C680, 7. / 3.) *
                    std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.)) -
               (C797 * C680 *
                    (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                     C715 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                         0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                         std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                    C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) +
                C796 * C680 *
                    (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                     C715 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                         0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                         std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                    C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1)) /
                   (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                    std::pow(std::pow(C680, 7. / 3.) *
                                 std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                             2))) +
              C802 * C680 *
                  (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                       0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                       std::pow(std::pow(C680, 7. / 6.) * C715, 2) +
                   (std::pow(std::pow(C680, 7. / 6.) * C715, 2) *
                        (C715 * 0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) *
                             std::exp(-0.00081290825 * std::sqrt(C690) /
                                      (std::pow(C680, 7. / 6.) * C715)) *
                             0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                             std::pow(std::pow(C680, 7. / 6.) * C715, 2) +
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                             0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.)) -
                    C715 *
                        std::exp(-0.00081290825 * std::sqrt(C690) /
                                 (std::pow(C680, 7. / 6.) * C715)) *
                        0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * 2 * C715 *
                        std::pow(C680, 14. / 6.)) /
                       std::pow(std::pow(C680, 7. / 6.) * C715, 4)) *
                  C690 /
                  (std::pow(C680, 7. / 3.) *
                   std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.))) *
                 C802) +
            (((C796 * C680 *
                   std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                   C715 * C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) * 4. *
                   (std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                        std::pow(C680, 7. / 3.) *
                        std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) * 2 *
                        std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) +
                    std::pow(std::pow(C680, 7. / 3.) *
                                 std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                             2) *
                        C685 * std::pow(C693, C685 - 1)) /
                   (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                    std::pow(
                        4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                            std::pow(
                                std::pow(C680, 7. / 3.) *
                                    std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                2),
                        2)) +
               C680 *
                   (((((0.01555 * C703 + 0.0007 * C705) - 0.0269) - 0.0048 * C696) - C716) * C684 *
                        (C684 - 1) * std::pow(C693, C684 - 2) / (2. * (std::pow(2., C682) - 1.)) -
                    (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                         std::pow(std::pow(C680, 7. / 3.) *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                  2) *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                         C715 * C690 * std::pow(C680, 7. / 3.) * C685 * (C685 - 1) *
                         std::pow(C693, C685 - 2) -
                     std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                         C715 * C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) *
                         4. *
                         (std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                              std::pow(C680, 7. / 3.) *
                              std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) * 2 *
                              std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) +
                          std::pow(
                              std::pow(C680, 7. / 3.) *
                                  std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                              2) *
                              C685 * std::pow(C693, C685 - 1)) /
                         (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.))) /
                        std::pow(
                            4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                std::pow(
                                    std::pow(C680, 7. / 3.) *
                                        std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                  2.),
                                    2),
                            2)) *
                   C797) -
              C794 * C680 *
                  (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                       C715 +
                   C690 * C715 * -0.00081290825 *
                       std::exp(-0.00081290825 * std::sqrt(C690) /
                                (std::pow(C680, 7. / 6.) * C715)) /
                       (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715)) *
                  std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) /
                  (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                   std::pow(std::pow(C680, 7. / 3.) *
                                std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                            2))) -
             C802 * C680 *
                 (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                  C715 *
                      std::exp(-0.00081290825 * std::sqrt(C690) /
                               (std::pow(C680, 7. / 6.) * C715)) *
                      0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                      std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                 C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) /
                 (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                  std::pow(std::pow(C680, 7. / 3.) *
                               std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                           2))) *
                C797;

        matrix.upper[13] =
            (((((C680 *
                     (((((0.01555 * C703 + 0.0007 * C705) - 0.0269) - 0.0048 * C696) - C716) *
                          C684 * (C684 - 1) * std::pow(C692, C684 - 2) /
                          (2. * (std::pow(2., C682) - 1.)) -
                      (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                           std::pow(
                               std::pow(C680, 7. / 3.) *
                                   std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                               2) *
                           std::exp(-0.00081290825 * std::sqrt(C690) /
                                    (std::pow(C680, 7. / 6.) * C715)) *
                           C715 * C690 * std::pow(C680, 7. / 3.) * C685 * (C685 - 1) *
                           std::pow(C692, C685 - 2) -
                       std::exp(-0.00081290825 * std::sqrt(C690) /
                                (std::pow(C680, 7. / 6.) * C715)) *
                           C715 * C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) *
                           4. *
                           (std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                std::pow(C680, 7. / 3.) *
                                std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) * 2 *
                                std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) +
                            std::pow(
                                std::pow(C680, 7. / 3.) *
                                    std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                2) *
                                C685 * std::pow(C692, C685 - 1)) /
                           (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.))) /
                          std::pow(
                              4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                  std::pow(
                                      std::pow(C680, 7. / 3.) *
                                          std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                    2.),
                                      2),
                              2)) *
                     C796 +
                 C797 * C680 *
                     std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                     C715 * C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) * 4. *
                     (std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                          std::pow(C680, 7. / 3.) *
                          std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) * 2 *
                          std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) +
                      std::pow(std::pow(C680, 7. / 3.) *
                                   std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                               2) *
                          C685 * std::pow(C692, C685 - 1)) /
                     (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                      std::pow(
                          4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                              std::pow(
                                  std::pow(C680, 7. / 3.) *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                  2),
                          2))) -
                C794 * C680 *
                    (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                         C715 +
                     C690 * C715 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) /
                         (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715)) *
                    std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) /
                    (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                     std::pow(std::pow(C680, 7. / 3.) *
                                  std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                              2))) -
               C802 * C680 *
                   (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                    C715 *
                        std::exp(-0.00081290825 * std::sqrt(C690) /
                                 (std::pow(C680, 7. / 6.) * C715)) *
                        0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                        std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                   C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) /
                   (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                    std::pow(std::pow(C680, 7. / 3.) *
                                 std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                             2))) *
                  C806 +
              ((C794 * C680 *
                    (C715 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) /
                         (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715) +
                     (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715 *
                          (C690 * C715 * 0.0000006608198229 *
                               std::exp(-0.00081290825 * std::sqrt(C690) /
                                        (std::pow(C680, 7. / 6.) * C715)) /
                               (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715) +
                           C715 * -0.00081290825 *
                               std::exp(-0.00081290825 * std::sqrt(C690) /
                                        (std::pow(C680, 7. / 6.) * C715))) -
                      C690 * C715 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C690) /
                                   (std::pow(C680, 7. / 6.) * C715)) *
                          std::pow(C680, 7. / 6.) * C715 / std::sqrt(C690)) /
                         std::pow(2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715, 2)) /
                    (std::pow(C680, 7. / 3.) *
                     std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.)) -
                (C797 * C680 *
                     (std::exp(-0.00081290825 * std::sqrt(C690) /
                               (std::pow(C680, 7. / 6.) * C715)) *
                          C715 +
                      C690 * C715 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C690) /
                                   (std::pow(C680, 7. / 6.) * C715)) /
                          (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715)) *
                     std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) +
                 C796 * C680 *
                     (std::exp(-0.00081290825 * std::sqrt(C690) /
                               (std::pow(C680, 7. / 6.) * C715)) *
                          C715 +
                      C690 * C715 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C690) /
                                   (std::pow(C680, 7. / 6.) * C715)) /
                          (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715)) *
                     std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1)) /
                    (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                     std::pow(std::pow(C680, 7. / 3.) *
                                  std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                              2))) +
               C802 * C680 *
                   ((std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                     C715 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                         0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                         std::pow(std::pow(C680, 7. / 6.) * C715, 2)) +
                    (-0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) /
                         (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715) +
                     C715 *
                         (std::exp(-0.00081290825 * std::sqrt(C690) /
                                   (std::pow(C680, 7. / 6.) * C715)) *
                              0.00081290825 * std::pow(C680, 7. / 6.) / (2 * std::sqrt(C690)) +
                          0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) *
                              -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C690) /
                                       (std::pow(C680, 7. / 6.) * C715)) /
                              (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715)) /
                         std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                        C690) /
                   (std::pow(C680, 7. / 3.) *
                    std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.))) *
                  C804) +
             ((C794 * C680 *
                   ((std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                     C715 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                         0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                         std::pow(std::pow(C680, 7. / 6.) * C715, 2)) +
                    (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715 * C690 *
                         (C715 * -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C690) /
                                       (std::pow(C680, 7. / 6.) * C715)) *
                              0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                              std::pow(std::pow(C680, 7. / 6.) * C715, 2) -
                          0.00081290825 * std::exp(-0.00081290825 * std::sqrt(C690) /
                                                   (std::pow(C680, 7. / 6.) * C715))) -
                     C690 * C715 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                         2 * std::sqrt(C690) * std::pow(C680, 7. / 6.)) /
                        std::pow(2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715, 2)) /
                   (std::pow(C680, 7. / 3.) *
                    std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.)) -
               (C797 * C680 *
                    (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                     C715 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                         0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                         std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                    C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) +
                C796 * C680 *
                    (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                     C715 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                         0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                         std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                    C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1)) /
                   (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                    std::pow(std::pow(C680, 7. / 3.) *
                                 std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                             2))) +
              C802 * C680 *
                  (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                       0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                       std::pow(std::pow(C680, 7. / 6.) * C715, 2) +
                   (std::pow(std::pow(C680, 7. / 6.) * C715, 2) *
                        (C715 * 0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) *
                             std::exp(-0.00081290825 * std::sqrt(C690) /
                                      (std::pow(C680, 7. / 6.) * C715)) *
                             0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                             std::pow(std::pow(C680, 7. / 6.) * C715, 2) +
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                             0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.)) -
                    C715 *
                        std::exp(-0.00081290825 * std::sqrt(C690) /
                                 (std::pow(C680, 7. / 6.) * C715)) *
                        0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * 2 * C715 *
                        std::pow(C680, 14. / 6.)) /
                       std::pow(std::pow(C680, 7. / 6.) * C715, 4)) *
                  C690 /
                  (std::pow(C680, 7. / 3.) *
                   std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.))) *
                 C812) +
            (((C796 * C680 *
                   std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                   C715 * C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) * 4. *
                   (std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                        std::pow(C680, 7. / 3.) *
                        std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) * 2 *
                        std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) +
                    std::pow(std::pow(C680, 7. / 3.) *
                                 std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                             2) *
                        C685 * std::pow(C693, C685 - 1)) /
                   (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                    std::pow(
                        4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                            std::pow(
                                std::pow(C680, 7. / 3.) *
                                    std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                2),
                        2)) +
               C680 *
                   (((((0.01555 * C703 + 0.0007 * C705) - 0.0269) - 0.0048 * C696) - C716) * C684 *
                        (C684 - 1) * std::pow(C693, C684 - 2) / (2. * (std::pow(2., C682) - 1.)) -
                    (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                         std::pow(std::pow(C680, 7. / 3.) *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                  2) *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                         C715 * C690 * std::pow(C680, 7. / 3.) * C685 * (C685 - 1) *
                         std::pow(C693, C685 - 2) -
                     std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                         C715 * C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) *
                         4. *
                         (std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                              std::pow(C680, 7. / 3.) *
                              std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) * 2 *
                              std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) +
                          std::pow(
                              std::pow(C680, 7. / 3.) *
                                  std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                              2) *
                              C685 * std::pow(C693, C685 - 1)) /
                         (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.))) /
                        std::pow(
                            4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                std::pow(
                                    std::pow(C680, 7. / 3.) *
                                        std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                  2.),
                                    2),
                            2)) *
                   C797) -
              C794 * C680 *
                  (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                       C715 +
                   C690 * C715 * -0.00081290825 *
                       std::exp(-0.00081290825 * std::sqrt(C690) /
                                (std::pow(C680, 7. / 6.) * C715)) /
                       (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715)) *
                  std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) /
                  (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                   std::pow(std::pow(C680, 7. / 3.) *
                                std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                            2))) -
             C802 * C680 *
                 (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                  C715 *
                      std::exp(-0.00081290825 * std::sqrt(C690) /
                               (std::pow(C680, 7. / 6.) * C715)) *
                      0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                      std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                 C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) /
                 (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                  std::pow(std::pow(C680, 7. / 3.) *
                               std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                           2))) *
                C807;

        matrix.upper[14] =
            (((((C680 *
                     (((((0.01555 * C703 + 0.0007 * C705) - 0.0269) - 0.0048 * C696) - C716) *
                          C684 * (C684 - 1) * std::pow(C692, C684 - 2) /
                          (2. * (std::pow(2., C682) - 1.)) -
                      (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                           std::pow(
                               std::pow(C680, 7. / 3.) *
                                   std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                               2) *
                           std::exp(-0.00081290825 * std::sqrt(C690) /
                                    (std::pow(C680, 7. / 6.) * C715)) *
                           C715 * C690 * std::pow(C680, 7. / 3.) * C685 * (C685 - 1) *
                           std::pow(C692, C685 - 2) -
                       std::exp(-0.00081290825 * std::sqrt(C690) /
                                (std::pow(C680, 7. / 6.) * C715)) *
                           C715 * C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) *
                           4. *
                           (std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                std::pow(C680, 7. / 3.) *
                                std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) * 2 *
                                std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) +
                            std::pow(
                                std::pow(C680, 7. / 3.) *
                                    std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                2) *
                                C685 * std::pow(C692, C685 - 1)) /
                           (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.))) /
                          std::pow(
                              4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                  std::pow(
                                      std::pow(C680, 7. / 3.) *
                                          std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                    2.),
                                      2),
                              2)) *
                     C806 +
                 C807 * C680 *
                     std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                     C715 * C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) * 4. *
                     (std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                          std::pow(C680, 7. / 3.) *
                          std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) * 2 *
                          std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) +
                      std::pow(std::pow(C680, 7. / 3.) *
                                   std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                               2) *
                          C685 * std::pow(C692, C685 - 1)) /
                     (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                      std::pow(
                          4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                              std::pow(
                                  std::pow(C680, 7. / 3.) *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                  2),
                          2))) -
                C804 * C680 *
                    (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                         C715 +
                     C690 * C715 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) /
                         (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715)) *
                    std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) /
                    (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                     std::pow(std::pow(C680, 7. / 3.) *
                                  std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                              2))) -
               C812 * C680 *
                   (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                    C715 *
                        std::exp(-0.00081290825 * std::sqrt(C690) /
                                 (std::pow(C680, 7. / 6.) * C715)) *
                        0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                        std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                   C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) /
                   (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                    std::pow(std::pow(C680, 7. / 3.) *
                                 std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                             2))) *
                  C806 +
              ((C804 * C680 *
                    (C715 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) /
                         (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715) +
                     (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715 *
                          (C690 * C715 * 0.0000006608198229 *
                               std::exp(-0.00081290825 * std::sqrt(C690) /
                                        (std::pow(C680, 7. / 6.) * C715)) /
                               (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715) +
                           C715 * -0.00081290825 *
                               std::exp(-0.00081290825 * std::sqrt(C690) /
                                        (std::pow(C680, 7. / 6.) * C715))) -
                      C690 * C715 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C690) /
                                   (std::pow(C680, 7. / 6.) * C715)) *
                          std::pow(C680, 7. / 6.) * C715 / std::sqrt(C690)) /
                         std::pow(2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715, 2)) /
                    (std::pow(C680, 7. / 3.) *
                     std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.)) -
                (C807 * C680 *
                     (std::exp(-0.00081290825 * std::sqrt(C690) /
                               (std::pow(C680, 7. / 6.) * C715)) *
                          C715 +
                      C690 * C715 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C690) /
                                   (std::pow(C680, 7. / 6.) * C715)) /
                          (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715)) *
                     std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) +
                 C806 * C680 *
                     (std::exp(-0.00081290825 * std::sqrt(C690) /
                               (std::pow(C680, 7. / 6.) * C715)) *
                          C715 +
                      C690 * C715 * -0.00081290825 *
                          std::exp(-0.00081290825 * std::sqrt(C690) /
                                   (std::pow(C680, 7. / 6.) * C715)) /
                          (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715)) *
                     std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1)) /
                    (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                     std::pow(std::pow(C680, 7. / 3.) *
                                  std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                              2))) +
               C812 * C680 *
                   ((std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                     C715 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                         0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                         std::pow(std::pow(C680, 7. / 6.) * C715, 2)) +
                    (-0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) /
                         (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715) +
                     C715 *
                         (std::exp(-0.00081290825 * std::sqrt(C690) /
                                   (std::pow(C680, 7. / 6.) * C715)) *
                              0.00081290825 * std::pow(C680, 7. / 6.) / (2 * std::sqrt(C690)) +
                          0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) *
                              -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C690) /
                                       (std::pow(C680, 7. / 6.) * C715)) /
                              (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715)) /
                         std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                        C690) /
                   (std::pow(C680, 7. / 3.) *
                    std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.))) *
                  C804) +
             ((C804 * C680 *
                   ((std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                     C715 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                         0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                         std::pow(std::pow(C680, 7. / 6.) * C715, 2)) +
                    (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715 * C690 *
                         (C715 * -0.00081290825 *
                              std::exp(-0.00081290825 * std::sqrt(C690) /
                                       (std::pow(C680, 7. / 6.) * C715)) *
                              0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                              std::pow(std::pow(C680, 7. / 6.) * C715, 2) -
                          0.00081290825 * std::exp(-0.00081290825 * std::sqrt(C690) /
                                                   (std::pow(C680, 7. / 6.) * C715))) -
                     C690 * C715 * -0.00081290825 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                         2 * std::sqrt(C690) * std::pow(C680, 7. / 6.)) /
                        std::pow(2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715, 2)) /
                   (std::pow(C680, 7. / 3.) *
                    std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.)) -
               (C807 * C680 *
                    (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                     C715 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                         0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                         std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                    C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) +
                C806 * C680 *
                    (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                     C715 *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                         0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                         std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                    C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1)) /
                   (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                    std::pow(std::pow(C680, 7. / 3.) *
                                 std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                             2))) +
              C812 * C680 *
                  (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                       0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                       std::pow(std::pow(C680, 7. / 6.) * C715, 2) +
                   (std::pow(std::pow(C680, 7. / 6.) * C715, 2) *
                        (C715 * 0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) *
                             std::exp(-0.00081290825 * std::sqrt(C690) /
                                      (std::pow(C680, 7. / 6.) * C715)) *
                             0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                             std::pow(std::pow(C680, 7. / 6.) * C715, 2) +
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                             0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.)) -
                    C715 *
                        std::exp(-0.00081290825 * std::sqrt(C690) /
                                 (std::pow(C680, 7. / 6.) * C715)) *
                        0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * 2 * C715 *
                        std::pow(C680, 14. / 6.)) /
                       std::pow(std::pow(C680, 7. / 6.) * C715, 4)) *
                  C690 /
                  (std::pow(C680, 7. / 3.) *
                   std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.))) *
                 C812) +
            (((C806 * C680 *
                   std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                   C715 * C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C692, C685 - 1) * 4. *
                   (std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                        std::pow(C680, 7. / 3.) *
                        std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) * 2 *
                        std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) +
                    std::pow(std::pow(C680, 7. / 3.) *
                                 std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                             2) *
                        C685 * std::pow(C693, C685 - 1)) /
                   (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                    std::pow(
                        4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                            std::pow(
                                std::pow(C680, 7. / 3.) *
                                    std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                2),
                        2)) +
               C680 *
                   (((((0.01555 * C703 + 0.0007 * C705) - 0.0269) - 0.0048 * C696) - C716) * C684 *
                        (C684 - 1) * std::pow(C693, C684 - 2) / (2. * (std::pow(2., C682) - 1.)) -
                    (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                         std::pow(std::pow(C680, 7. / 3.) *
                                      std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                                  2) *
                         std::exp(-0.00081290825 * std::sqrt(C690) /
                                  (std::pow(C680, 7. / 6.) * C715)) *
                         C715 * C690 * std::pow(C680, 7. / 3.) * C685 * (C685 - 1) *
                         std::pow(C693, C685 - 2) -
                     std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                         C715 * C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) *
                         4. *
                         (std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                              std::pow(C680, 7. / 3.) *
                              std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) * 2 *
                              std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) +
                          std::pow(
                              std::pow(C680, 7. / 3.) *
                                  std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                              2) *
                              C685 * std::pow(C693, C685 - 1)) /
                         (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.))) /
                        std::pow(
                            4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                                std::pow(
                                    std::pow(C680, 7. / 3.) *
                                        std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) /
                                                  2.),
                                    2),
                            2)) *
                   C807) -
              C804 * C680 *
                  (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) *
                       C715 +
                   C690 * C715 * -0.00081290825 *
                       std::exp(-0.00081290825 * std::sqrt(C690) /
                                (std::pow(C680, 7. / 6.) * C715)) /
                       (2 * std::sqrt(C690) * std::pow(C680, 7. / 6.) * C715)) *
                  std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) /
                  (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                   std::pow(std::pow(C680, 7. / 3.) *
                                std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                            2))) -
             C812 * C680 *
                 (std::exp(-0.00081290825 * std::sqrt(C690) / (std::pow(C680, 7. / 6.) * C715)) +
                  C715 *
                      std::exp(-0.00081290825 * std::sqrt(C690) /
                               (std::pow(C680, 7. / 6.) * C715)) *
                      0.00081290825 * std::sqrt(C690) * std::pow(C680, 7. / 6.) /
                      std::pow(std::pow(C680, 7. / 6.) * C715, 2)) *
                 C690 * std::pow(C680, 7. / 3.) * C685 * std::pow(C693, C685 - 1) /
                 (4. * std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.) *
                  std::pow(std::pow(C680, 7. / 3.) *
                               std::sqrt((std::pow(C692, C685) + std::pow(C693, C685)) / 2.),
                           2))) *
                C807;
    }
}

} // namespace excgrid
