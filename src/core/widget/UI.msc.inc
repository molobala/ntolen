// Generated automatically from src/core/widget/UI.msc. Do not edit.
R"------(
dunan kulu Renderer {

    dunan clear()

    dunan rect(x, y, w, h)

    dunan fill(x, y, w, h)

    dunan line(x, y, w, h)

    dunan color(x, y, w, h)

    dunan point(x, y)

    dunan complete()

}

dunan kulu Window {

    dilan kura(title, w, h) {

        A.yira("Creating window")

    }

    dunan show()

    dunan hide()

    dunan renderer

    dunan onUpdate=(fn)

    onUpdate(fn) {

        ale.onUpdate = fn

    }

}
)------";
