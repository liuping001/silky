//
// Created by liuping on 2019/4/2.
//

#pragma once

struct no_copyable {
    no_copyable (const no_copyable &) = delete;
    no_copyable operator = (const no_copyable &) = delete;
};
