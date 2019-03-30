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
    let lwipHeader = path.resolve(__dirname, '../lwip/lwip/src/include');
    let lwipIp4Header = path.resolve(__dirname, '../lwip/lwip/src/include/ipv4');
    let lwipIp6Header = path.resolve(__dirname, '../lwip/lwip/src/include/ipv6');
    let contribHeader = path.resolve(__dirname, '../lwip/contrib/ports/unix/include');
    let customHeader = path.resolve(__dirname, '../lwip/custom');
    let distDir = path.resolve(__dirname, '../out/lwip/include');
    await fs.copy(lwipHeader, distDir, {overwrite: false, errorOnExist: true});
    await fs.copy(lwipIp4Header, distDir, {overwrite: false, errorOnExist: true});
    await fs.copy(lwipIp6Header, distDir, {overwrite: false, errorOnExist: true});
    await fs.copy(contribHeader, distDir, {overwrite: false, errorOnExist: true});
    await fs.copy(customHeader, distDir, {overwrite: false, errorOnExist: true});
}

async function buildThird() {
    let build = path.resolve(__dirname, '../out');
    await fs.remove(build);
    await buildLwip();
    await copyLwipHeader();
    await buildLibuv();
}

buildThird();