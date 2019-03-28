const path = require('path');

const fs = require('fs-extra');

async function copyHeader() {
    let lwipHeader = path.resolve(__dirname, '../lwip/lwip-2.1.2/src/include');
    let contribHeader = path.resolve(__dirname, '../lwip/contrib-2.1.0/ports/unix/port/include');
    let customHeader = path.resolve(__dirname, '../lwip/custom');
    let distDir = path.resolve(__dirname, '../out/lwip/include');
    await fs.copy(lwipHeader, distDir, {overwrite: false, errorOnExist: true});
    await fs.copy(contribHeader, distDir, {overwrite: false, errorOnExist: true});
    await fs.copy(customHeader, distDir, {overwrite: false, errorOnExist: true});
}

copyHeader();