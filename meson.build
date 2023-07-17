project('stax', 'c', version: '0.1.0')

deps = []
deps += dependency('SDL2')
deps += dependency('SDL2_ttf')

src = []
src += 'src/stax.c'
src += 'src/piece.c'
src += 'src/board.c'
src += 'src/SDL_ext.c'
src += 'src/ui_number.c'

executable('stax', src, dependencies: deps, include_directories: 'include')

