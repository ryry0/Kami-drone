let SessionLoad = 1
if &cp | set nocp | endif
let s:cpo_save=&cpo
set cpo&vim
imap <2-LeftMouse> <Nop>
imap <LeftMouse> <Nop>
nnoremap <silent>  :TmuxNavigateUp
nnoremap <silent> <NL> :TmuxNavigateDown
nnoremap  :call SmoothScroll(0)
nnoremap <silent>  :TmuxNavigateLeft
nnoremap <NL> 
nnoremap  
nnoremap <silent>  :TmuxNavigateRight
nnoremap  :next
nnoremap  :prev
nnoremap  :call SmoothScroll(1)
nnoremap  y "+y
nnoremap  Y "+Y
nmap  # :call NumberToggle() 
vnoremap  ( `>a)`<i(
nmap  = gg=G
nmap  [ =i{
nmap  { =i{
nmap  N :cp
nmap  n :cn
nmap  m :w:make!
nnoremap  X rXjrYjrZ
nnoremap  x rxjryjrz
nmap  d 
nmap  i 
nmap  v 
nnoremap  s :mks! session.vim
nnoremap  5 :%
nnoremap  W :w !sudo tee %
nnoremap  w :w
nnoremap  Q :q!
nnoremap  q :q
vnoremap  p 
nnoremap  p 
nmap  	 :%s/\s\+$// 
nnoremap  l :ls
nnoremap  f :ls:b 
nmap  e :e#
noremap  ; g;g;
vnoremap  Y "+Y
vnoremap  y "+y
onoremap  Y "+Y
onoremap  y "+y
vnoremap  t 
vnoremap  ] 
nnoremap  t 
nnoremap  ] 
nmap <silent>  O O
nmap <silent>  o o
nmap <silent>   i
nmap <silent>    :nohlsearch 
nnoremap : :set norelativenumber | set number:
vnoremap K <Nop>
nnoremap K kJ
nnoremap Q <Nop>
nmap _ds :call SetStatusLineStyle()
nnoremap ci[ %ci[
nnoremap ci{ %ci{
nnoremap ci( %ci(
nnoremap di[ %di[
nnoremap di{ %di{
nnoremap di( %di(
nmap gx <Plug>NetrwBrowseX
vnoremap j gj
nnoremap j gj
vnoremap k gk
nnoremap k gk
nnoremap q; q:
nnoremap vi[ %vi[
nnoremap vi{ %vi{
nnoremap vi( %vi(
nnoremap <silent> <Plug>NetrwBrowseX :call netrw#NetrwBrowseX(expand("<cWORD>"),0)
nnoremap <F5> :call NumberToggle() 
nnoremap <F12> :!ctags -R --c++-kinds=+p --fields=+iaS --extra=+q 
nnoremap <F1> <Nop>
map <ScrollWheelDown> 
map <ScrollWheelUp> 
nnoremap <F2> :set invpaste paste?
vmap <2-LeftMouse> <Nop>
nmap <2-LeftMouse> <Nop>
vmap <LeftMouse> <Nop>
nmap <LeftMouse> <Nop>
inoremap  :call SmoothScroll(0)i
cnoremap  <Down>
cnoremap  <Up>
inoremap  :call SmoothScroll(1)i
cnoremap <expr> %% getcmdtype() == ':' ? expand('%:h').'/' : '%%'
inoremap JK 
inoremap Jk 
inoremap Kj 
inoremap KJ 
inoremap jK 
inoremap jk 
cnoremap jk 
inoremap kJ 
inoremap kj 
cnoremap kj 
cnoreabbr <expr> Q ((getcmdtype() is# ':' && getcmdline() is# 'Q')?('q'):('Q'))
cnoreabbr <expr> B ((getcmdtype() is# ':' && getcmdline() is# 'B')?('b'):('B'))
let &cpo=s:cpo_save
unlet s:cpo_save
set autoindent
set backspace=indent,eol,start
set cinoptions=N-s
set copyindent
set directory=~/.temp/swp//
set expandtab
set exrc
set fileencodings=ucs-bom,utf-8,default,latin1
set formatoptions=croqlt
set helplang=en
set hidden
set history=50
set hlsearch
set ignorecase
set incsearch
set laststatus=2
set nomodeline
set mouse=a
set pastetoggle=<F2>
set path=/mnt/c/Users/Ryan/Google\ Drive/Work/Triolo\ Lab/Programs/hybridFrontEnd_27June16/serial_tools/**
set ruler
set runtimepath=~/.vim,/var/lib/vim/addons,/usr/share/vim/vimfiles,/usr/share/vim/vim74,/usr/share/vim/vimfiles/after,/var/lib/vim/addons/after,~/.vim/after
set scrolloff=2
set shiftwidth=2
set shortmess=at
set showcmd
set showmatch
set smartcase
set smartindent
set softtabstop=2
set spelllang=en_us
set splitbelow
set splitright
set nostartofline
set statusline=%f\ %y%([%R%M]%)%{'!'[&ff=='unix']}%{'$'[!&list]}%{'~'[&pm=='']}%=buff:#%n\ line:%l/%L\ col:%c%V\ 
set suffixes=.bak,~,.swp,.o,.info,.aux,.log,.dvi,.bbl,.blg,.brf,.cb,.ind,.idx,.ilg,.inx,.out,.toc
set tabstop=2
set tags=./tags;/
set textwidth=80
set titlestring=%t%(\ [%R%M]%)
set undodir=~/.temp/und//
set undofile
set wildignore=*.a,*.o,*.aux,*.brf,*.out,*.x,*.bmp,*.gif,*.ico,*.jpg,*.png,.DS_Store,.git,.hg,.svn,*.pdf,*~,*.swp,*.tmp
set wildignorecase
set wildmenu
set wildmode=longest:full,full
set window=65
let s:so_save = &so | let s:siso_save = &siso | set so=0 siso=0
let v:this_session=expand("<sfile>:p")
silent only
cd /mnt/c/Users/Ryan/Google\ Drive/Work/Triolo\ Lab/Programs/hybridFrontEnd_27June16/serial_tools
if expand('%') == '' && !&modified && line('$') <= 1 && getline(1) == ''
  let s:wipebuf = bufnr('%')
endif
set shortmess=aoO
badd +15 read_serial.c
badd +80 ../packet.h
badd +1 ../serial.h
badd +1 ../gui.c
badd +18 makefile
badd +0 ../packet.c
args read_serial.c ../packet.h
edit ../serial.h
set splitbelow splitright
wincmd _ | wincmd |
vsplit
1wincmd h
wincmd _ | wincmd |
split
1wincmd k
wincmd w
wincmd w
wincmd t
set winheight=1 winwidth=1
exe '1resize ' . ((&lines * 31 + 33) / 66)
exe 'vert 1resize ' . ((&columns * 65 + 66) / 132)
exe '2resize ' . ((&lines * 31 + 33) / 66)
exe 'vert 2resize ' . ((&columns * 65 + 66) / 132)
exe 'vert 3resize ' . ((&columns * 66 + 66) / 132)
argglobal
edit ../serial.h
setlocal keymap=
setlocal noarabic
setlocal autoindent
setlocal nobinary
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal cindent
setlocal cinkeys=0{,0},0),:,0#,!^F,o,O,e
setlocal cinoptions=N-s
setlocal cinwords=if,else,while,do,for,switch
setlocal colorcolumn=
setlocal comments=sO:*\ -,mO:*\ \ ,exO:*/,s1:/*,mb:*,ex:*/,://
setlocal commentstring=/*%s*/
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal copyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
set cursorline
setlocal cursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'cpp'
setlocal filetype=cpp
endif
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
setlocal foldmethod=manual
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=croql
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=0
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
set linebreak
setlocal linebreak
setlocal nolisp
setlocal nolist
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal nomodeline
setlocal modifiable
setlocal nrformats=octal,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=ccomplete#Complete
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
set relativenumber
setlocal norelativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal shiftwidth=2
setlocal noshortname
setlocal smartindent
setlocal softtabstop=2
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en_us
setlocal statusline=
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=3000
if &syntax != 'cpp'
setlocal syntax=cpp
endif
setlocal tabstop=2
setlocal tags=
setlocal textwidth=80
setlocal thesaurus=
setlocal undofile
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
silent! normal! zE
let s:l = 19 - ((15 * winheight(0) + 15) / 31)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
19
normal! 0
wincmd w
argglobal
2argu
edit ../packet.c
iabbr <buffer> <silent> ifnd =MapNoContext(' ifnd', '#ifndef\<CR\>#define\<CR\>#endif\<ESC\>2kA\<C-R\>=Eatchar()\<CR\>')
iabbr <buffer> <silent> bb =MapNoContext(' bb', '{\<CR\>}\<ESC\>O\<C-R\>=Eatchar()\<CR\>')
iabbr <buffer> <silent> intmainb =MapNoContext(' intmainb', 'int main() {\<CR\>}\<ESC\>O\<C-R\>=Eatchar()\<CR\>')
iabbr <buffer> <silent> intmain =MapNoContext(' intmain', 'int main(int argc, char ** argv) {\<CR\>}\<ESC\>O\<C-R\>=Eatchar()\<CR\>')
iabbr <buffer> <silent> swb =MapNoContext(' swb', 'switch () {\<CR\>\<CR\>}\<UP\>\<UP\>\<ESC\>0f(a\<C-R\>=Eatchar()\<CR\>')
iabbr <buffer> <silent> elb =MapNoContext(' elb', 'else {\<CR\>}\<ESC\>O\<C-R\>=Eatchar()\<CR\>')
iabbr <buffer> <silent> elib =MapNoContext(' elib', 'else if () {\<CR\>\<CR\>}\<UP\>\<UP\>\<ESC\>0f(a\<C-R\>=Eatchar()\<CR\>')
iabbr <buffer> <silent> ifb =MapNoContext(' ifb', 'if () {\<CR\>\<CR\>}\<UP\>\<UP\>\<ESC\>0f(a\<C-R\>=Eatchar()\<CR\>')
iabbr <buffer> <silent> ifelse =MapNoContext(' ifelse', 'if () {\<CR\>\<CR\>}\<CR\>else {\<CR\>\<CR\>}\<ESC\>5k0f(a\<C-R\>=Eatchar()\<CR\>')
iabbr <buffer> <silent> whil =MapNoContext(' whil', 'while () {\<CR\>\<CR\>}\<UP\>\<UP\>\<ESC\>0f(a\<C-R\>=Eatchar()\<CR\>')
iabbr <buffer> <silent> forb =MapNoContext(' forb', 'for (;;) {\<CR\>\<CR\>}\<UP\>\<UP\>\<ESC\>0f(a\<C-R\>=Eatchar()\<CR\>')
iabbr <buffer> <silent> fors =MapNoContext(' fors', 'for (size_t i = 0; i \<; ++i) {\<CR\>\<CR\>}\<UP\>\<UP\>\<ESC\>0f\<a \<C-R\>=Eatchar()\<CR\>')
iabbr <buffer> <silent> foru =MapNoContext(' foru', 'for (unsigned int i = 0; i \<; ++i) {\<CR\>\<CR\>}\<UP\>\<UP\>\<ESC\>0f\<a \<C-R\>=Eatchar()\<CR\>')
iabbr <buffer> <silent> fori =MapNoContext(' fori', 'for (int i = 0; i \<; ++i) {\<CR\>\<CR\>}\<UP\>\<UP\>\<ESC\>0f\<a \<C-R\>=Eatchar()\<CR\>')
iabbr <buffer> <silent> defi =MapNoContext(' defi', '#define \<C-R\>=Eatchar()\<CR\>')
iabbr <buffer> <silent> incl =MapNoContext(' incl', '#include \<\>\<Left\>\<C-R\>=Eatchar()\<CR\>')
setlocal keymap=
setlocal noarabic
setlocal autoindent
setlocal nobinary
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal cindent
setlocal cinkeys=0{,0},0),:,0#,!^F,o,O,e
setlocal cinoptions=N-s
setlocal cinwords=if,else,while,do,for,switch
setlocal colorcolumn=
setlocal comments=sO:*\ -,mO:*\ \ ,exO:*/,s1:/*,mb:*,ex:*/,://
setlocal commentstring=/*%s*/
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal copyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
set cursorline
setlocal cursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'c'
setlocal filetype=c
endif
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
setlocal foldmethod=manual
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=croqlt
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=0
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
set linebreak
setlocal linebreak
setlocal nolisp
setlocal nolist
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal nomodeline
setlocal modifiable
setlocal nrformats=octal,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=ccomplete#Complete
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
set relativenumber
setlocal relativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal shiftwidth=2
setlocal noshortname
setlocal smartindent
setlocal softtabstop=2
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en_us
setlocal statusline=
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=3000
if &syntax != 'c'
setlocal syntax=c
endif
setlocal tabstop=2
setlocal tags=
setlocal textwidth=80
setlocal thesaurus=
setlocal undofile
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
silent! normal! zE
let s:l = 32 - ((18 * winheight(0) + 15) / 31)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
32
normal! 052|
wincmd w
argglobal
edit ../packet.h
setlocal keymap=
setlocal noarabic
setlocal autoindent
setlocal nobinary
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal cindent
setlocal cinkeys=0{,0},0),:,0#,!^F,o,O,e
setlocal cinoptions=N-s
setlocal cinwords=if,else,while,do,for,switch
setlocal colorcolumn=
setlocal comments=sO:*\ -,mO:*\ \ ,exO:*/,s1:/*,mb:*,ex:*/,://
setlocal commentstring=/*%s*/
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal copyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
set cursorline
setlocal cursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'cpp'
setlocal filetype=cpp
endif
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
setlocal foldmethod=manual
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=croql
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=0
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
set linebreak
setlocal linebreak
setlocal nolisp
setlocal nolist
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal nomodeline
setlocal modifiable
setlocal nrformats=octal,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=ccomplete#Complete
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
setlocal norelativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal shiftwidth=2
setlocal noshortname
setlocal smartindent
setlocal softtabstop=2
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en_us
setlocal statusline=
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=3000
if &syntax != 'cpp'
setlocal syntax=cpp
endif
setlocal tabstop=2
setlocal tags=
setlocal textwidth=80
setlocal thesaurus=
setlocal undofile
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
silent! normal! zE
let s:l = 3 - ((2 * winheight(0) + 31) / 63)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
3
normal! 0
wincmd w
3wincmd w
exe '1resize ' . ((&lines * 31 + 33) / 66)
exe 'vert 1resize ' . ((&columns * 65 + 66) / 132)
exe '2resize ' . ((&lines * 31 + 33) / 66)
exe 'vert 2resize ' . ((&columns * 65 + 66) / 132)
exe 'vert 3resize ' . ((&columns * 66 + 66) / 132)
tabedit ../gui.c
set splitbelow splitright
wincmd t
set winheight=1 winwidth=1
argglobal
edit ../gui.c
setlocal keymap=
setlocal noarabic
setlocal autoindent
setlocal nobinary
setlocal bufhidden=
setlocal buflisted
setlocal buftype=
setlocal cindent
setlocal cinkeys=0{,0},0),:,0#,!^F,o,O,e
setlocal cinoptions=N-s
setlocal cinwords=if,else,while,do,for,switch
setlocal colorcolumn=
setlocal comments=sO:*\ -,mO:*\ \ ,exO:*/,s1:/*,mb:*,ex:*/,://
setlocal commentstring=/*%s*/
setlocal complete=.,w,b,u,t,i
setlocal concealcursor=
setlocal conceallevel=0
setlocal completefunc=
setlocal copyindent
setlocal cryptmethod=
setlocal nocursorbind
setlocal nocursorcolumn
set cursorline
setlocal cursorline
setlocal define=
setlocal dictionary=
setlocal nodiff
setlocal equalprg=
setlocal errorformat=
setlocal expandtab
if &filetype != 'c'
setlocal filetype=c
endif
setlocal foldcolumn=0
setlocal foldenable
setlocal foldexpr=0
setlocal foldignore=#
setlocal foldlevel=0
setlocal foldmarker={{{,}}}
setlocal foldmethod=manual
setlocal foldminlines=1
setlocal foldnestmax=20
setlocal foldtext=foldtext()
setlocal formatexpr=
setlocal formatoptions=croql
setlocal formatlistpat=^\\s*\\d\\+[\\]:.)}\\t\ ]\\s*
setlocal grepprg=
setlocal iminsert=0
setlocal imsearch=0
setlocal include=
setlocal includeexpr=
setlocal indentexpr=
setlocal indentkeys=0{,0},:,0#,!^F,o,O,e
setlocal noinfercase
setlocal iskeyword=@,48-57,_,192-255
setlocal keywordprg=
set linebreak
setlocal linebreak
setlocal nolisp
setlocal nolist
setlocal makeprg=
setlocal matchpairs=(:),{:},[:]
setlocal nomodeline
setlocal modifiable
setlocal nrformats=octal,hex
set number
setlocal number
setlocal numberwidth=4
setlocal omnifunc=ccomplete#Complete
setlocal path=
setlocal nopreserveindent
setlocal nopreviewwindow
setlocal quoteescape=\\
setlocal noreadonly
set relativenumber
setlocal norelativenumber
setlocal norightleft
setlocal rightleftcmd=search
setlocal noscrollbind
setlocal shiftwidth=2
setlocal noshortname
setlocal smartindent
setlocal softtabstop=2
setlocal nospell
setlocal spellcapcheck=[.?!]\\_[\\])'\"\	\ ]\\+
setlocal spellfile=
setlocal spelllang=en_us
setlocal statusline=
setlocal suffixesadd=
setlocal swapfile
setlocal synmaxcol=3000
if &syntax != 'c'
setlocal syntax=c
endif
setlocal tabstop=2
setlocal tags=
setlocal textwidth=80
setlocal thesaurus=
setlocal undofile
setlocal nowinfixheight
setlocal nowinfixwidth
setlocal wrap
setlocal wrapmargin=0
silent! normal! zE
let s:l = 97 - ((10 * winheight(0) + 10) / 21)
if s:l < 1 | let s:l = 1 | endif
exe s:l
normal! zt
97
normal! 0
3wincmd w
tabnext 1
if exists('s:wipebuf')
  silent exe 'bwipe ' . s:wipebuf
endif
unlet! s:wipebuf
set winheight=1 winwidth=20 shortmess=at
let s:sx = expand("<sfile>:p:r")."x.vim"
if file_readable(s:sx)
  exe "source " . fnameescape(s:sx)
endif
let &so = s:so_save | let &siso = s:siso_save
doautoall SessionLoadPost
unlet SessionLoad
" vim: set ft=vim :
