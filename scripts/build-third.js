const path = require('path');
const util = require('util');
const { spawn } = require('child_process');

const fs = require('fs-extra');

async function buildLibuv() {
    let builddir = path.resolve(__dirname, '../out/libuv');
    await fs.mkdirp(builddir);
    await promiseSpawm('cmake ../../libuv-1.27.0', builddir);
    await promiseSpawm('make uv', builddir);
}

async function buildLwip() {
    let builddir = path.resolve(__dirname, '../out/lwip');
    await fs.mkdirp(builddir);
    await promiseSpawm('cmake ../../lwip', builddir);
    await promiseSpawm('make', builddir);

}

async function copyLibuvHeader() {
    let lwipHeader = path.resolve(__dirname, '../libuv-1.27.0/include');
    let distDir = path.resolve(__dirname, '../out/libuv/include');
    await fs.copy(lwipHeader, distDir, {overwrite: false, errorOnExist: true});
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

async function buildThird() {
    let build = path.resolve(__dirname, '../out');
    await fs.remove(build);
    await buildLwip();
    await copyLwipHeader();
    await buildLibuv();
    await copyLibuvHeader();
}

function promiseSpawm(cmd, cwd) {
    return new Promise((resolve, reject) => {

        let part = cmd.split(' ');
        let app = part[0];
        part.splice(0,1);
        let args = part;
        console.log('运行', app, args,cwd);
        const ls = spawn(app, args, {
            shell : true,
            env : process.env,
            stdio : 'pipe',
            cwd,
        });
        ls.on('error', (err) => {
            console.error(err);
            reject(err);
        });
        if (ls.stdout) {
            ls.stdout.on('data', (data) => {
                console.log(data.toString('utf-8'));
            });
        }

        if (ls.stderr) {
            ls.stderr.on('data', (data) => {
                console.log(data.toString('utf-8'));
            });
        }

        ls.on('close', (code) => {
            resolve(code);
        });
    });

}



buildThird();