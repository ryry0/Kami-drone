with import <nixpkgs> {}; {
	hybridUIEnv = stdenv.mkDerivation {
		name = "nuklear-env";
		shellHook = ''
		export RPROMPT=[desktop_ui-shell]$RPROMPT
		'';

		buildInputs = [
		        x11
		        mesa
		        SDL2
		        SDL2_gfx
		        SDL2_image
		        SDL2_mixer
		        SDL2_net
		        SDL2_ttf
		];
	};
}

