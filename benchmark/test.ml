open Core
open Core_bench

external mk_pair_caml   : 'a -> 'b -> 'a * 'b = "mk_pair_caml"
external mk_pair_caml_slow : 'a -> 'b -> 'a * 'b = "mk_pair_caml_slow"
external mk_pair_mlroot : 'a -> 'b -> 'a * 'b = "mk_pair_mlroot"

let () =
  Command.run (Bench.make_command [
    Bench.Test.create ~name:"mk_pair_caml"
      (fun () -> ignore (mk_pair_caml 1 2));
    Bench.Test.create ~name:"mk_pair_caml_slow"
      (fun () -> ignore (mk_pair_caml_slow 1 2));
    Bench.Test.create ~name:"mk_pair_mlroot"
      (fun () -> ignore (mk_pair_mlroot 1 2));
  ])
