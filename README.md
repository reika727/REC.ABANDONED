# THIS REPOSITORY IS ARCHIVED!!
あんまりにも行き当たりばったりに作りすぎたので途中で行き詰ってしまいました。[こちら](https://github.com/reika727/REC)のほうで一から作り直しています。

# REC
C言語をアセンブリ言語に翻訳します。未完成です。とりあえずC89の仕様を満たしつつ64bitマシン上で動くことを目標にする。

## Overview
大体コンパイラのセオリーに則っていると思います。
1. 字句解析。入力されたソースコードを分解してトークン列に変換する。
1. 構文解析。トークン列から抽象構文木を作る。
1. 意味解析。右辺値への代入など意味的な不正を検出する。
1. コード生成。アセンブリを出力する。

## 現在の文法
<pre>
  <code>
    translation unit        = { function definition } ;
    function definition     = type specifier, identifier, argument list, compound ;
    argument list           = "(", ( "void" | argument declarelations ), ")" ;
    argument declarelations = type specifier, identifier, { ",", type specifier, identifier } ;
    statement               = expression statement | null statement | compound | control statement ;
    expression statement    = expression, ";" ;
    null statement          = ";" ;
    compound                = "{", { variable definition }, { statement }, "}" ;
    control statement       = selection statement | iteration statement | jump statement ;
    selection statement     = if else ;
    iteration statement     = while | for ;
    jump statement          = break | continue | return ;
    if else                 = "if", "(", expression, ")", statement, [ "else", statement ] ;
    while                   = "while", "(", expression, ")", statement ;
    for                     = "for", "(", [ expression ], ";", [ expression ], ";", [ expression ], ")", statement ;
    break                   = "break", ";" ;
    continue                = "continue", ";" ;
    return                  = "return", [ expression ], ";" ;
    variable definition     = type specifier, identifier, [ "=", expression ], { ",", identifier, [ "=", expression ] }, ";" ;
    expression              = (そのうち書く) ;
    identifier              = ( "_" | alphabet )+, { "_" | alphabet | digit } ;
    alphabet                = "A" | (中略) | "z" ;
    digit                   = "0" | (中略) | "9" ;
    type specifier          = "int" ;
  </code>
</pre>

## Example
<details>
  <summary>長いので折り畳み</summary>
    こんな感じのソースファイルexample.cがあるとします。10までの階乗を計算します。
    <pre>
      <code>
        int main()
        {
            int a=1,i;
            for(i=1;i<=10;++i){
                a*=i;
                __builtin_print_u64(a);
                __builtin_put_ascii(10);
            }
            return 0;
        }
      </code>
    </pre>
    __builtin_print_u64というのは名前の通りビルトイン関数です。符号なし64bit整数を標準出力に書き出します。
    将来的にはprintfなどを使いたいのですが、まだstdio.hは解析できないのでこれで妥協します。
    また__builtin_put_asciiは(名前からわかると思いますが)asciiコードで指定した文字を一文字だけ出力します。
    10を指定すると改行というわけです。<br />
    さて、これをこうします(recはソースを適当にコンパイルしてすでに作成済みであるとします)。<br />
    example.cを入力として受け取り、アセンブリに翻訳したものをexample.sに出力します。<br />
    それをexample.outという実行ファイルにします。ビルトイン関数のソースコードも一緒にコンパイルするのをお忘れなく。
    <pre>
      <code>
        $ ./rec example.c example.s
        $ gcc -o example.out example.s builtin_func.s
        $ ./example.out
        1
        2
        6
        24
        120
        720
        5040
        40320
        362880
        3628800
      </code>
    </pre>
    計算できてました。
</details>

## RECの機能
<details>
  <summary>こっちも長いので折り畳み</summary>
  ✅は実装済み、🔵は未実装。

  ### データ
  ✅64bitローカル符号付き整数型変数の定義<br />
  🔵signedとunsignedの区別<br />
  🔵グローバル変数<br />
  🔵実数型<br />
  🔵ポインタ型<br />
  🔵列挙型<br />
  🔵構造体<br />
  🔵共用体<br />

  ### 制御文
  ✅if文、else文<br />
  🔵switch文<br />
  ✅while文<br />
  🔵do-while文<br />
  ✅for文<br />
  ✅break文<br />
  ✅continue文<br />
  🔵goto文<br />
  ✅return文<br />

  ### 関数
  ✅定義<br />
  🔵プロトタイプ宣言<br />

  ### 演算子

  #### 算術演算子
  ✅単項プラス<br />
  ✅単項マイナス<br />
  ✅加算<br />
  ✅減算<br />
  ✅乗算<br />
  ✅除算<br />
  ✅剰余<br />
  ✅前置インクリメント<br />
  ✅前置デクリメント<br />
  ✅後置インクリメント<br />
  ✅後置デクリメント<br />
  ✅加算代入<br />
  ✅減算代入<br />
  ✅乗算代入<br />
  ✅除算代入<br />
  ✅剰余代入<br />

  #### 比較演算子
  ✅等価<br />
  ✅非等価<br />
  ✅小なり<br />
  ✅大なり<br />
  ✅小なりイコール<br />
  ✅大なりイコール<br />

  #### 論理演算子
  ✅論理否定<br />
  ✅論理和<br />
  ✅論理積<br />

  #### ビット演算子
  🔵ビット否定<br />
  🔵ビット和<br />
  🔵ビット積<br />
  🔵ビット排他的論理和<br />
  🔵左シフト<br />
  🔵右シフト<br />
  🔵ビット和代入<br />
  🔵ビット積代入<br />
  🔵ビット排他的論理和代入<br />
  🔵左シフト代入<br />
  🔵右シフト代入<br />

  #### その他
  ✅単純代入<br />
  ✅関数呼び出し<br />
  ✅カンマ<br />
  🔵条件<br />
  🔵配列添え字<br />
  🔵間接<br />
  🔵アドレス<br />
  🔵直接メンバ<br />
  🔵間接メンバ<br />
  🔵キャスト<br />
  🔵sizeof<br />

  ### その他
  ✅コメント<br />
  🔵int main(int argc, char*argv[])<br />
</details>

## 謝辞
ほぼ[ここ](https://www.sigbus.info/compilerbook)に書いてあることをやってるだけです。マジ感謝。
