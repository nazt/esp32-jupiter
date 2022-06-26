const glob = require("glob"),
    path = require("path")

async function main() {
    for (let file of glob.sync(`${__dirname}/setup/*.{js,ts}`)) {
        console.log("------------------------------")
        console.log("> Executing", path.basename(file))
        console.log("------------------------------")
        await require(file)()
    }
}

main()
