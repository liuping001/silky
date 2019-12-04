//
// Created by liuping on 2019/3/19.
//

#pragma once

/**
 * 代替
 * do {
 *
 * } while(false);
 *
 */
#define SCOPE for (size_t my_inner_i = 0; my_inner_i < 1; my_inner_i++)

#define IF(condition) if (condition) SCOPE

#define ELSE else SCOPE

#define ELSE_IF(condition) else if (condition) SCOPE