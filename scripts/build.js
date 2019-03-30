const path = require('path');
const util = require('util');
const { exec } = require('child_process');

const fs = require('fs-extra');

const execp = util.promisify(exec);

async function buildLibuv() {

}

async function buildLwip() {
    let builddir = path.resolve(__dirname, '../out/lwip');
    await fs.mkdirp(builddir);
    await execp('cmake ../../lwip', {cwd: builddir});
    await execp('make', {cwd: builddir});

}

async function copyLwipHeader() {
    let lwipHeader = path.resolve(__dirname, '../lwip/lwip-2.1.2/src/include');
    let contribHeader = path.resolve(__dirname, '../lwip/contrib-2.1.0/ports/unix/port/include');
    let customHeader = path.resolve(__dirname, '../lwip/custom');
    let distDir = path.resolve(__dirname, '../out/lwip/include');
    await fs.copy(lwipHeader, distDir, {overwrite: false, errorOnExist: true});
    await fs.copy(contribHeader, distDir, {overwrite: false, errorOnExist: true});
    await fs.copy(customHeader, distDir, {overwrite: false, errorOnExist: true});
}

async function build() {
    let build = path.resolve(__dirname, '../out');
    await fs.remove(build);
    await buildLwip();
    await copyLwipHeader();
    await buildLibuv();
}

build();